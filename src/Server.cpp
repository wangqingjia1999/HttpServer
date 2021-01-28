#include "Server.hpp"

Server::Server()
    : logger(new Logger()),
      thread_pool(std::make_shared<ThreadPool>()),
      m_connection(std::make_shared<Connection>()),
      server_socket(new ServerSocket()),
      m_resource_handler(new ResourceHandler()),
      m_configuration(new ServerConfiguration())
{
    m_resource_handler->set_resource_directory_path(m_configuration->get_resource_directory_path());
}

Server::~Server()
{
}

void Server::listen_at(const std::string& host, const int port)
{
    Server_Socket_State server_socket_state;
    for(;;)
    {
        server_socket_state = server_socket->listen_at(host, port);
        switch(server_socket_state)
        {
            case Server_Socket_State::UNINITIALIZED:
            {
                continue;
            }
                
            case Server_Socket_State::READABLE:
            {   
                request_core_handler(server_socket->read_from());
                
                if(!server_socket->write_to(m_connection->get_response()->generate_response()))
                    printf("Error in sending m_connection->get_response()");

                break;
            }
                            
            case Server_Socket_State::ERROR:
            {
                break;
            }
                
            default:
            {
                break;
            }   
        }
    }
}

bool Server::parse_request(const std::string& raw_request_string)
{
    m_connection->get_request()->set_raw_request(raw_request_string);
    return m_connection->get_request()->parse_raw_request();
}

std::string Server::get_raw_request()
{
    return m_connection->get_request()->get_raw_request();
}

std::string Server::get_raw_response()
{
    return m_connection->get_response()->generate_response();
}

void Server::set_raw_request(const std::string& raw_request)
{
    m_connection->get_request()->set_raw_request(raw_request);
}

bool Server::handle_post_request()
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

void Server::request_core_handler(const std::string& raw_request_string)
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
        if(m_connection->get_request()->get_request_uri().find("resource/audios") == std::string::npos)
            StatusHandler::handle_status_code(m_connection->get_response(), 405);
        

        return;
    }
    else if(m_connection->get_request()->get_request_method() == "PUT")
    {
        if(m_connection->get_request()->get_request_uri().find("resource/audios") == std::string::npos)
            StatusHandler::handle_status_code(m_connection->get_response(), 405);

        return;
    }
    else if(m_connection->get_request()->get_request_method() == "HEAD")
    {

        return;
    }
}