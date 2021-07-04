#include "Server.hpp"

Server::Server()
    : m_connection{ std::make_shared<Connection>() },
      m_thread_pool{ std::make_shared<ThreadPool>() },
      m_sqlite_handler{ new SqliteHandler() },
      m_server_socket{ new ServerSocket() },
      m_resource_handler{ new ResourceHandler() },
      m_configuration{ new ServerConfiguration() }
{
}

Server::~Server()
{
}

void Server::listen_at(const std::string& host, const int port)
{
    Server_Socket_State server_socket_state;
    for(;;)
    {
        server_socket_state = m_server_socket->listen_at(host, port);
        switch(server_socket_state)
        {
            case Server_Socket_State::UNINITIALIZED:
            {
                continue;
            }
                
            case Server_Socket_State::READABLE:
            {   
                request_core_handler(m_server_socket->read_from());
                
                if(!m_server_socket->write_to(m_connection->get_response()->generate_response()))
                    Logger::warn("Error in sending response message.");

                m_connection->get_request()->clear_up();                
                m_connection->get_response()->clear_up();

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