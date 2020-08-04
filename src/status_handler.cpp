#include "status_handler.hpp"

namespace status_handler
{                           
    void handle_status_code(std::shared_ptr< Message::Response >& response, const int status_code)
    {
        switch(status_code)
        {
            case 101:
            {
                response->set_status(101);
                response->set_reason_phrase(101);
                response->add_header("Upgrade", "websocket");
                response->add_header("Connection", "Upgrade");
                break;
            }

            case 200:
            {
                response->set_status(200);
                response->set_reason_phrase(200);
                response->add_header("Accept-Ranges", "bytes");
                response->add_header("Content-Length", response->get_body_length());
                response->add_header("Content-Type", "text/html");
                // add_header("Date", "Mon, 3 Jul 2020 12:28:53 GMT");
                break;
            }

            case 400:	// bad request
            {
                response->set_status(400);
                response->set_reason_phrase(400);
                response->add_header("Content-Type", "text/html");
                response->set_body("<html><h1> 400 Bad Request :( </h1></html>");
                break;
            }

            case 404:
            {
                response->set_status(404);
                response->set_reason_phrase(404);
                response->add_header("Content-Type", "text/html");
                response->set_body("<html><h1> 404 Not Found :( </h1></html>");
                break;
            }
            
            case 405:	// Method Not Allowed
            {
                response->set_status(405);
                response->set_reason_phrase(405);
                response->add_header("Content-Type", "text/html");
                response->add_header("Allow", "GET, HEAD, PUT");
                response->set_body("<html><h1> 405 Method Not Allowed :( </h1></html>");
                break;
            }

            case 500:	// Internal Server Error
            {
                response->set_status(500);
                response->set_reason_phrase(500);
                response->add_header("Content-Type", "text/html");
                response->set_body("<html><h1> 500 Internal Server Error :( </h1></html>\r\n");
                break;
            }

        }

    }
}