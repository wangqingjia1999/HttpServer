#include "StatusHandler.hpp"
#include "ServerConfiguration.hpp"

namespace StatusHandler
{                           
    void handle_status_code(std::shared_ptr< Message::Response >& response, const int status_code)
    {
        response->set_status(status_code);
        response->set_reason_phrase(status_code);

        // The Date header MUST be sent if the server is capable of generating accurate date.
        response->add_header("Date", Timer::get_current_http_time());

        // Generally, a server should have the Server header despite the status.
        response->add_header("Server", "Bitate");

        response->add_header("Host", "www.bitate.com");
        
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

                // TODO: Support HTTP/2.0
                response->add_header("Upgrade", "HTTP/2.0");
                
                break;
            }

            /**
             * 2xx: successfuly
             */
            case 200:   // OK
            {
                response->add_header("Content-Type", response->get_content_type());
                
                break;
            }

            case 201:   // Created
            { 
                break;
            }

            case 202:   // Accepted
            {
                break;
            }

            case 203:   // Non-Authoritative Information
            {
                break;
            }

            case 204:   // No Content
            {
                break;
            }
            
            case 205:   // Reset Content
            {
                break;
            }
            
            case 206:   // Partial Content
            {
                break;
            }

            /**
             * 3xx: redirection
             */
            case 300:   // Multiple Choices
            {

            }

            case 301:   // Moved Permanently
            {
                break;
            }

            case 302:   // Found
            {
                break;
            }

            case 303:   // See Other
            {
                break;
            }

            case 304:   // Not Modified
            {
                break;
            }

            case 307:   // Temporary Redirect
            {
                break;
            }
            
            case 308:   // Permanent Redirect
            {
                break;
            }

            /**
             * 4xx: client error
             */
            case 400:   // bad request
            {
                /**
                 * The request could not be parsed by server due to the malformed request message.
                 */
                response->set_body("<html> Bad Request :< </html>");
                response->add_header("Content-Type", "text/html");
                break;
            }

            case 401:   // Unauthorized
            {
                /**
                 * The request requires user authentication.
                 * The response MUST include a 'WWW-Authenticate' header containing at least one challenge.
                 */
                response->add_header("WWW-Authenticate", "Basic; realm=\"Secured area\"");

                break;
            }

            case 403:   // Forbidden
            {
                break;
            }

            case 404:   // Not Found
            {
                response->set_body(
                    "<html>"
                        "<head>"
                            "<title>"
                                "No results"
                            "</title>"
                        "</head>"
                        "<body>"
                            "<center>"
                                "<h1>"
                                    "No Results Found"
                                "</h1>"
                                "<div>"
                                    "<a href=\"/\">Try Another Query :^)</a>"
                                "</div>"
                            "</center>"
                        "</body>"
                    "</html>"
                );
                response->add_header("Content-Type", "text/html");
                break;
            }

            case 405:   // Method Not Allowed
            {
                response->add_header("Allow", "GET");
                response->set_body(
                    "<html>"
                        "<head>"
                            "<title>"
                                "405 Method Not Allowed"
                            "</title>"
                        "</head>"
                        "<body>"
                            "<center>"
                                "<h1> Please ensure the request method is compliant with the requested resource :^) </h1>"
                            "</center>"
                            "<center>"
                                "<h5> Bitate Server </h5>"
                            "</center>"
                        "</body>"
                    "</html>"
                );
                response->add_header("Content-Type", "text/html");
                break;
            }

            case 406:   // Not Acceptable
            {
                break;
            }

            case 407:   // Proxy Authentication Required
            {
                break;
            }

            case 408:   // Request Timeout
            {
                break;
            }

            case 409:   // Conflict
            {
                break;
            }

            case 410:   // Gone
            {
                break;
            }

            case 411:   // Length Required
            {
                break;
            }

            case 412:   // Precondition Failed
            {
                break;
            }

            case 413:   // Payload Too Large
            {
                break;
            }

            case 414:   // URI Too Long
            {
                break;
            }
            
            case 415:	// Unsupported Media Type
            {
                break;
            }

            case 416:	// Range Not Satisfiable
            {
                break;
            }

            case 417:	// Expected Failed
            {
                break;
            }

            case 418:	// I'm A Teapot(Not Coffee Pot)
            {
                break;
            }

            case 426:	// Upgrade Required
            {
                break;
            }

            case 428:	// Precondition Required
            {
                break;
            }

            case 429:	// Too Many Requests
            {
                break;
            }

            case 431:	// Request Header Field Too Large
            {
                break;
            }

            /**
             * 5xx: server error
             */
            case 500:	// Internal Server Error
            {
                response->set_body("<html> Internal server error! </html>");
                break;
            }

            case 501:   // Not Implemented
            {
                response->set_body("<html> Request Method Not Implemented. </html>");
                response->add_header("Content-Type", "text/html");
                break;
            }

            case 502:   // Bad Gateway
            {
                break;
            }

            case 503:   // Service Unavailable
            {
                break;
            }

            case 504:   // Gateway Timeout
            {
                break;
            }

            case 505:   // HTTP Version Not Supported
            {
                break;
            }
        }

        response->add_header("Content-Length", std::to_string(response->get_body().size()));
    }
}
