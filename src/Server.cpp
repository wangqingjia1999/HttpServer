#include "Server.hpp"

Server::~Server()
{
}
Server::Server()
    : logger(new Logger()),
      thread_pool(std::make_shared<Thread_Pool>()),
      request(std::make_shared<Message::Request>()),
      response(std::make_shared<Message::Response>()),
      server_socket(new Server_Socket()),
      m_configuration(new ServerConfiguration())
{
}

bool Server::is_websocket_opening_handshake(std::shared_ptr< Message::Request >& request)
{
    if(request->get_header("Upgrade") == "websocket")
    {
        return true;
    }
    return false;
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
                request_core_handler(  
                    server_socket->read_from(
                        server_socket->get_readable_fd()
                    )
                );
                
                if(!server_socket->write_to(
                        server_socket->get_readable_fd(),
                        response->get_response_message()
                    )
                )
                {
                    // TODO: log sending error
                    printf("Error in sending response");
                }
                break;
            }
                            
            case Server_Socket_State::ERROR:
            {
                std::cout << "socket error" << std::endl;
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
    request->set_raw_request(raw_request_string);
    return request->parse_raw_request();
}

std::string Server::get_raw_request()
{
    return request->get_raw_request();
}

std::string Server::get_raw_response()
{
    return response->get_response_message();
}

void Server::set_raw_request(const std::string& raw_request)
{
    request->set_raw_request(raw_request);
}

bool Server::handle_post_request()
{
    /**
     * e.g. request->get_body() is "Name=Bitate&Age=21&Email=admin%40bitate.com&Password=qwerqwer"
     */
    Percent_Encoding percent_encoding;
    /**
     * Append an additional '&', so that each name=value pair has a trailing '&'
     */
    std::string body_buffer = percent_encoding.decode(request->get_body()) + '&';

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
    // Parse request
    if(!parse_request(raw_request_string))
    {
        // 400 Bad Request
        // FIXME: thread_pool->post_task( [this]{ logger.record_error_message("parse request"); } );
        Status_Handler::handle_status_code(response, 400);
        return;
    }

    // If it is POST request
    if(request->get_request_method() == "POST")
    {
        if(handle_post_request())
        {
            // Content created
            Status_Handler::handle_status_code(response, 200);
            response->add_header("Location", "/sign_up_done.html");
            response->set_content("/sign_up_done.html");
            response->add_header("Content-Type", "text/html");
            response->add_header("Content-Length", response->get_body_length());
            return;
        }
        else
        {
            // bad request
            // FIXME: thread_pool->post_task( [this]{ logger.record_error_message("parse POST request"); } );
            Status_Handler::handle_status_code(response, 400);
        }
    }

    // If it is websocket opening handshake
    if(
        request->has_header("Upgrade")
        && request->get_header("Upgrade") == "websocket"
    )
    {
        WebSocket websocket(request, response);
        if(!websocket.parse_websocket_request())
        {
            // FIXME: thread_pool->post_task( [this]{ logger.record_error_message("parse WebSocket request"); } );
            return;
        }
        response->add_header("Sec-WebSocket-Accept", websocket.generate_sec_websocket_key());
        Status_Handler::handle_status_code(response, 101);
        return;
    }

    // Resources handler
    if(
        request->get_request_method() == "GET"
        && !response->set_content(request->get_request_uri())
    )
    {
        // 404 Not Found
        Status_Handler::handle_status_code(response, 404);
        // FIXME: thread_pool->post_task( [this]{ logger.record_error_message("handle requested resources"); } );
        return;
    }

    // Status handler
    Status_Handler::handle_status_code(response, 200);
}