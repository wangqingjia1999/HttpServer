#include "Server.hpp"

Server::~Server()
{
}
Server::Server()
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

bool Server::listen_at(const std::string& host, const int port)
{
    server_socket.listen_at(host, port);
    return true; // TODO
}

void Server::send_response(const int client_socket_fd, const std::string& buffer)
{
    server_socket.write_to(client_socket_fd, buffer.c_str(), buffer.size());
}

void Server::receive_request(const int client_socket_fd)
{
    server_socket.read_from(client_socket_fd, receive_buffer, receive_buffer_length);
}

bool Server::parse_request()
{
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

int Server::get_client_fd()
{
    return client_socket_fd;
}

int Server::get_server_fd()
{
    return server_socket_fd;
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
    URI::Percent_Encoding percent_encoding;
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

    // add data to mysql
    // ...
    
    return true;
}

void Server::request_core_handler()
{
    // Parse request
    if(!parse_request())
    {
        // 400 Bad Request
        thread_pool->post_task( [this]{ logger.record_error_message("parse request"); } );
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
            thread_pool->post_task( [this]{ logger.record_error_message("parse POST request"); } );
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
            thread_pool->post_task( [this]{ logger.record_error_message("parse WebSocket request"); } );
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
        thread_pool->post_task( [this]{ logger.record_error_message("handle requested resources"); } );
        return;
    }

    // Status handler
    Status_Handler::handle_status_code(response, 200);
}