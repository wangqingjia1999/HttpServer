#include "Worker.hpp"
#include "Logger.hpp"
#include "UnixDomainHelper.hpp"
#include "StatusHandler.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/epoll.h>

namespace
{
    /**
     * Epoll triggered events max size
     */
    constexpr size_t EPOLL_TRIGGERED_EVENTS_MAX_SIZE = 1024;

    /**
     * Epoll interest/event list size
     */
    constexpr size_t EPOLL_INTEREST_LIST_SIZE = 1024;

    /**
     * Maximum socket listening buffer size in byte
     */
    constexpr size_t MAXIMUM_LISTENING_PENDING_QUEUE = 4096;

    /**
     * Maximum sending/receiving buffer size in byte
     */
    constexpr size_t MAXIMUM_BUFFER_SIZE = 8192;
}

Worker::Worker(
    const int worker_socket,
    const std::string& accept_mutex_name,
    const std::string& worker_mutex_name
)
    : m_epfd{ -1 },
      m_worker_socket{ worker_socket },
      m_accept_mutex_name{ accept_mutex_name },
      m_worker_mutex_name{ worker_mutex_name },
      m_worker_socket_handler{ new WorkerSocket() },
      m_connection{ std::make_shared<Connection>() },
      m_sqlite_handler{ new SqliteHandler() },
      m_server_socket{ new WorkerSocket() },
      m_resource_handler{ new ResourceHandler() },
      m_configuration{ new ServerConfiguration() }
{
    m_epfd = epoll_create(EPOLL_INTEREST_LIST_SIZE);
    if(m_epfd == -1)
    {
        Logger::error("worker epoll_create() error: " + std::string{ strerror(errno) });
        throw std::runtime_error("worker epoll_create() error");
    }

    epoll_event worker_socket_event;
    worker_socket_event.data.fd = m_worker_socket;
    worker_socket_event.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_worker_socket, &worker_socket_event) == -1)
    {
        Logger::error("worker epoll add error: " + std::string{ strerror(errno) });
        throw std::runtime_error("worker epoll add error: " + std::string{ strerror(errno) });
    }

    m_accept_mutex = sem_open(accept_mutex_name.c_str(), 0);
    if(m_accept_mutex == SEM_FAILED)
    {
        Logger::error("sem_open() in worker error: " + std::string{ strerror(errno) });
        throw std::runtime_error("sem_open() error");
    }

    m_worker_mutex = sem_open(m_worker_mutex_name.c_str(), 0);
    if(m_worker_mutex == SEM_FAILED)
    {
        Logger::error("sem_open() in worker error: " + std::string{ strerror(errno) });
        throw std::runtime_error("sem_open() in worker error");
    }
}

Worker::~Worker()
{
    close(m_epfd);

    sem_close(m_worker_mutex);
    sem_close(m_accept_mutex);
    
    delete m_accept_mutex;
    m_accept_mutex = nullptr;

    delete m_worker_mutex;
    m_worker_mutex = nullptr;
}

void Worker::event_loop()
{
    int sum = 0;
    struct epoll_event triggered_events[EPOLL_TRIGGERED_EVENTS_MAX_SIZE] = { 0 };
    bool has_accepted_fd = false;
    for(;;)
    {
        if(!has_accepted_fd)
        {
            sem_wait(m_accept_mutex);
            sem_wait(m_worker_mutex);

            char send_buffer[] = "";
            int send_result = send(m_worker_socket, &send_buffer, sizeof(send_buffer), 0);
            if(send_result == -1)
                has_accepted_fd = false;
            else
                has_accepted_fd = true;
        }
        
        // wait for master's response;
        // if, at the same time, we have clients that are readable, process them firstly.
        switch(sum = epoll_wait(m_epfd, triggered_events, EPOLL_TRIGGERED_EVENTS_MAX_SIZE, -1)) 
        {
            case -1:
            {
                Logger::error("worker epoll_ctl() error: " + std::string(strerror(errno)));
                continue;
            }

            default:
            {
                for(int i = 0; i < sum; ++i)
                {
                    int triggered_fd = triggered_events[i].data.fd;
                    uint32_t triggered_event =  triggered_events[i].events;
                    
                    // master dispatches fd
                    if((triggered_event & EPOLLIN) && (triggered_fd == m_worker_socket))
                    {
                        int accepted_socket = UnixDomainHelper::read_fd(m_worker_socket);
                        sem_post(m_worker_mutex);
                        if(accepted_socket == -1)
                        {
                            Logger::error("worker read accepted fd from master error");
                            continue;
                        }
                        
                        epoll_event new_client_event;
                        new_client_event.data.fd = accepted_socket;
                        new_client_event.events = EPOLLIN | EPOLLET;

                        if(epoll_ctl(m_epfd, EPOLL_CTL_ADD, accepted_socket, &new_client_event) == -1)
                        {
                            Logger::error("worker epoll add error: " + std::string{ strerror(errno) });
                            close(accepted_socket);
                            has_accepted_fd = false;
                        }
                    }

                    // accepted fd is readable
                    if((triggered_event & EPOLLIN) && (triggered_fd != m_worker_socket))
                    {
                        // It peer shudown wrting
                        if(triggered_event & EPOLLRDHUP)
                        {
                            close(triggered_fd);
                            Logger::debug("client shutdown wrting.");
                            continue;
                        }

                        if(m_worker_socket_handler->read_from(triggered_fd))
                        {
                            request_core_handler(m_worker_socket_handler->get_receive_buffer_string());
                        }
                        else
                        {
                            close(triggered_fd);
                            has_accepted_fd = false;
                            continue;
                        }

                        if(!m_server_socket->write_to(triggered_fd, m_connection->get_response()->generate_response()))
                            Logger::error("write to client error");

                        m_connection->get_request()->clear_up();                
                        m_connection->get_response()->clear_up();

                        close(triggered_fd);
                        has_accepted_fd = false;
                    }

                    if(triggered_event & EPOLLERR)
                    {
                        Logger::error("triggered socket event error: "+ std::string{ strerror(errno) });
                        close(triggered_fd);
                        has_accepted_fd = false;
                    }
                }
            }
        }   
    }
}

bool Worker::parse_request(const std::string& raw_request_string)
{
    m_connection->get_request()->set_raw_request(raw_request_string);
    return m_connection->get_request()->parse_raw_request();
}

std::string Worker::get_raw_request()
{
    return m_connection->get_request()->get_raw_request();
}

std::string Worker::get_raw_response()
{
    return m_connection->get_response()->generate_response();
}

void Worker::set_raw_request(const std::string& raw_request)
{
    m_connection->get_request()->set_raw_request(raw_request);
}

bool Worker::handle_post_request()
{
    PercentEncoding percent_encoding;
    
    /**
     * Append an additional '&', so that each name=value pair has a trailing '&'
     */
    std::string body_buffer = percent_encoding.decode(m_connection->get_request()->get_body()) + '&';

    while(!body_buffer.empty())
    {
        if(body_buffer.find_first_of('&') != std::string::npos)
        {
            if(body_buffer.find_first_of('=') != std::string::npos)
            {   
                std::string name = body_buffer.substr(0, body_buffer.find_first_of('='));
                
                std::string value = body_buffer.substr(
                    body_buffer.find_first_of('=') + 1, 
                    body_buffer.find_first_of('&') - body_buffer.find_first_of('=') - 1
                );

                post_data_map[std::move(name)] = std::move(value);

                body_buffer = body_buffer.substr(body_buffer.find_first_of('&')+1);
            }
        }
    }
    return true;
}

void Worker::request_core_handler(const std::string& raw_request_string)
{
    if(!parse_request(raw_request_string))
    {
        StatusHandler::handle_status_code(m_connection->get_response(), 400);
        return;
    }

    if(m_connection->get_request()->get_request_method() == "GET")
    {
        if(!m_resource_handler->fetch_resource(m_connection))
            StatusHandler::handle_status_code(m_connection->get_response(), 404);
        else
            StatusHandler::handle_status_code(m_connection->get_response(), 200);

        return;
    }
    else if(m_connection->get_request()->get_request_method() == "POST")
    {   
        if(m_connection->get_request()->get_header("Content-Type") == "application/x-www-form-urlencoded")
        {
            if(!handle_post_request())
                StatusHandler::handle_status_code(m_connection->get_response(), 400);

            if(post_data_map.find("username") != post_data_map.end())
            {
                UserInfo user_info;
                user_info.m_name = post_data_map["username"];
                user_info.m_password = post_data_map["password"];
                user_info.m_age = post_data_map["age"];
                user_info.m_email = post_data_map["email"];

                if(!m_sqlite_handler->add_new_user(user_info))
                    StatusHandler::handle_status_code(m_connection->get_response(), 400);
                else
                    StatusHandler::handle_status_code(m_connection->get_response(), 200);
                
                return;            
            }
        }
        
        return;
    }
    else if(m_connection->get_request()->get_request_method() == "PUT")
    {
        if(m_connection->get_request()->get_request_uri()->get_path_string().find("resource/audios") == std::string::npos)
            StatusHandler::handle_status_code(m_connection->get_response(), 405);

        return;
    }
    else if(m_connection->get_request()->get_request_method() == "HEAD")
    {
        return;
    }
}