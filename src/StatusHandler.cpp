#include "StatusHandler.hpp"

namespace StatusHandler
{                           
    void handle_status_code(std::shared_ptr< Message::Response >& response, const int status_code)
    {
        response->set_status(status_code);
        response->set_reason_phrase(status_code);

        // The Date header MUST be sent if the server is capable of generating accurate date.
        response->add_header("Date", Timer::get_current_http_time());
        // Generally, a server should have the following headers despite the status.
        response->add_header("Server", "Bitate");

        switch(status_code)
        {
            /**
             * 1xx: informational
             */
            case 100:   // Continue
            {
                break;
            }

            case 101:   // Switching Protocol
            {
                /**
                 * https://stackoverflow.com/a/61122084/11850070
                 */
                response->add_header("Connection", "upgrade");
                response->add_header("Upgrade", "HTTP/2.0");
                
                break;
            }

            /**
             * 2xx: successfuly
             */
            case 200:   // OK
            {
                response->add_header("Content-Length", response->get_body_length());
                response->add_header("Content-Type", response->get_content_type());
                
                break;
            }

            case 201:   //
            { 
    
                break;
            }

            case 202:   //
            {
                break;
            }

            case 203:   //
            {
                break;
            }

            case 204:   //
            {
                break;
            }
            
            case 205:   //
            {
                break;
            }
            
            case 206:   //
            {
                break;
            }

            /**
             * 3xx: redirection
             */
            case 301:   //
            {
                break;
            }

            case 302:   //
            {
                break;
            }

            case 303:   //
            {
                break;
            }

            case 304:   //
            {
                break;
            }

            case 305:   //
            {
                break;
            }

            case 306:   //
            {
                break;
            }
            
            case 307:   //
            {
                break;
            }
            
            case 308:   //
            {
                break;
            }

            /**
             * 4xx: client error
             */
            case 400:   // bad request
            {
                response->set_body("<html> Bad Request :< </html>");
                response->add_header("Content-Type", response->get_content_type());
                response->add_header("Content-Length", response->get_body_length());
                break;
            }

            case 401:   // 
            {
                break;
            }

            case 402:   // 
            {
                break;
            }

            case 403:   // 
            {
                break;
            }

            case 404:   // 
            {
                response->set_body(
                    "<html>"
                        "<head>"
                            "<title>"
                                "404 Not Found"
                            "</title>"
                        "</head>"
                        "<body>"
                            "<center>"
                                "<h1>404 Not Found</h1>"
                            "</center>"
                            "<center>"
                                "Bitate/0.0.1"
                            "</center>"
                        "</body>"
                    "</html>"
                );

                response->add_header("Content-Length", response->get_body_length());
                response->add_header("Content-Type", "text/html");

                break;
            }

            case 405:   // 
            {
                break;
            }

            case 406:   // 
            {
                break;
            }

            case 407:   // 
            {
                break;
            }

            case 408:   // 
            {
                break;
            }

            case 409:   // 
            {
                break;
            }

            case 410:   // 
            {
                break;
            }

            case 411:   // 
            {
                break;
            }

            case 412:   // 
            {
                break;
            }

            case 413:   // 
            {
                break;
            }

            case 414:   // 
            {
                break;
            }
            
            case 415:	// 
            {
                break;
            }

            case 416:	// 
            {
                break;
            }

            case 417:	// 
            {
                break;
            }

            case 418:	// 
            {
                break;
            }

            case 421:	// 
            {
                break;
            }

            case 422:	// 
            {
                break;
            }

            case 423:	// 
            {
                break;
            }

            case 424:	// 
            {
                break;
            }

            case 425:	// 
            {
                break;
            }

            case 426:	// 
            {
                break;
            }

            case 428:	// 
            {
                break;
            }

            case 429:	// 
            {
                break;
            }

            case 431:	//  
            {
                break;
            }

            case 451:	//  
            {
                break;
            }

            /**
             * 5xx: server error
             */
            case 500:	// Internal Server Error
            {
                break;
            }

            case 501:   //
            {
                break;
            }

            case 502:   //
            {
                break;
            }

            case 503:   //
            {
                break;
            }

            case 504:   //
            {
                break;
            }

            case 505:   //
            {
                break;
            }

            case 506:   //
            {
                break;
            }

            case 507:   //
            {
                break;
            }

            case 508:   //
            {
                break;
            }

            case 510:   //
            {
                break;
            }

            case 511:   //
            {
                break;
            }
        }

        response->generate_response();
    }
}
