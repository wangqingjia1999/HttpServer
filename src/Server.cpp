#include "Server.hpp"

Server::~Server()
{
}
Server::Server()
    : logger(new Logger()),
      thread_pool(std::make_shared<ThreadPool>()),
      request(std::make_shared<Message::Request>()),
      response(std::make_shared<Message::Response>()),
      server_socket(new ServerSocket()),
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
                request_core_handler( server_socket->read_from() );
                
                if(!server_socket->write_to( response->get_response_message() ))
                {
                    printf("Error in sending response");
                }

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
    PercentEncoding percent_encoding;
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
        StatusHandler::handle_status_code(response, 400);
        return;
    }

    if(request->get_request_method() == "GET")
    {

    }
    else if(request->get_request_method() == "POST")
    {

    }
    else if(request->get_request_method() == "HEAD")
    {

    }

    // Resources handler
}