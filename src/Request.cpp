#include "Uri.hpp"
#include "Request.hpp"
#include <sstream>

namespace Message
{
    struct Message::Request::Impl
    {
        std::shared_ptr< Uri::Uri > uri = std::make_shared< Uri::Uri > ();
        
        // Generated/Received raw request string
        std::string raw_request;

        // Request method. e.g. GET, POST
        std::string method;
        
        // default http_version
        std::string http_version = "HTTP/1.1";

        // Store the generated/received raw headers
        std::string headers;

        // For more info, see: https://tools.ietf.org/html/rfc2616#section-5.1.2
        std::string request_uri;

        // Contain the header {key: value} pairs
        std::map< std::string, std::string> headers_map;

        // request body string
        std::string body = "";
    };

    // lifecycle management
    Message::Request::~Request() = default;
    Message::Request::Request() : impl_(new Impl)
    {
    }

    Message::Request::Request(const Request& other) noexcept
    {
        *this = other;
    }
    Request& Message::Request::operator=(const Request& other) noexcept
    {
        if(this!=&other)
        {
            *impl_ = *other.impl_;
        }
        return *this;
    }
    Message::Request::Request(Request&&) noexcept = default;
    Request& Message::Request::operator=(Request&& ) noexcept = default;

    // public methods
    bool Message::Request::set_raw_request(std::string rawRequestString)
    {
        impl_->raw_request = rawRequestString;
        return true;
    }

    bool Message::Request::generate_request()
    {   
        if(!set_method())
        {
            return false;
        }

        if(!set_http_version())
        {
            return false;
        }

        if(!set_user_agent())
        {
            return false;
        }
        
        std::stringstream rawRequestStream;
        // set status line
        rawRequestStream << impl_->method << " " << impl_->uri->get_path_string() << " " << impl_->http_version << "\r\n";
        // set headers
        for(auto position = impl_->headers_map.cbegin(); position != impl_->headers_map.cend(); ++position)
        {
            rawRequestStream << position->first.c_str() << ": " << position->second.c_str() << "\r\n";    
        }
        // set headers end delimiter
        rawRequestStream << "\r\n";
        // set body 
        rawRequestStream << impl_->body;

        impl_->raw_request = rawRequestStream.str();
        return true;
    }

    bool Message::Request::parse_raw_request()
    {
        auto request_line_end_delimiter = impl_->raw_request.find("\r\n");
        if (request_line_end_delimiter == std::string::npos)
        {
            return false;
        }

        std::string request_line = impl_->raw_request.substr(0, request_line_end_delimiter);
        if (!parse_request_line(request_line))
        {
            return false;
        }

        std::string request_without_request_line = impl_->raw_request.substr(request_line_end_delimiter + 2);
        
        auto headers_end_delimiter = impl_->raw_request.find("\r\n\r\n");
        if (headers_end_delimiter == std::string::npos)
        {
            return false;
        }

        // +2 is to include the \r\n at the end of the last header
        impl_->headers = request_without_request_line.substr(0, headers_end_delimiter + 2);

        if (!parse_headers(impl_->headers))
        {
            return false;
        }

        if(request_without_request_line == "\r\n")
        {
            request_without_request_line.clear();
            return true;
        }

        impl_->body = request_without_request_line.substr(headers_end_delimiter + 4);

        return true;
    }

    bool Message::Request::parse_headers(const std::string& headers)
    {
        if (headers.find("\r\n") == std::string::npos)
        {
            impl_->headers.clear();
            return true;
        }

        std::string headers_buffer = headers;

        
        for(;;)
        {
            auto single_header_line_end_delimiter = headers_buffer.find("\r\n");
            
            // special case, only pass in "\r\n"
            if(headers_buffer == "\r\n")
            {
                impl_->headers.clear();
                return true;
            }
            
            if (single_header_line_end_delimiter != std::string::npos)
            {
                std::string single_header_line = headers_buffer.substr(0, single_header_line_end_delimiter);

                auto colonPosition = single_header_line.find(':');

                if (colonPosition == std::string::npos)
                {
                    impl_->headers.clear();
                    return false;
                }

                
                std::string header_name = single_header_line.substr(0, colonPosition);
                std::string headerValue = single_header_line.substr(colonPosition + 1);

                // TODO: add new function to strip leading and trailing whitespace from string
                // Any number of spaces or tabs may be between the ":" and the value. 
                // Header lines beginning with space or tab are actually part of the previous header line,
                // folded into multiple lines for easy reading.
                if (header_name.find_last_of(' ') == header_name.size())
                {
                    header_name = header_name.substr(0, header_name.find_last_of(' '));
                }
                else if(header_name.find_first_of(' ') == 0)
                {
                    header_name = header_name.substr(1);
                }

                if (headerValue.find_first_of(' ') == 0)
                {
                    headerValue = headerValue.substr(1);
                }
                else if (headerValue.find_last_of(' ') == headerValue.size())
                {
                    headerValue = headerValue.substr(0, headerValue.find_last_of(' '));
                }
            
                impl_->headers_map[header_name] = headerValue;
                
                headers_buffer = headers_buffer.substr(single_header_line_end_delimiter + 2);
            }
            else
            {
                break;
            }

        }

        return true;
    }

    bool Message::Request::parse_request_line(const std::string& request_line)
    {
        if (request_line.find(' ') == std::string::npos)
        {
            return false;
        }

        auto first_space_position = request_line.find_first_of(' ');
        auto second_space_position = request_line.find_last_of(' ');

        impl_->method = request_line.substr(0, first_space_position);
        impl_->request_uri = request_line.substr(first_space_position+1, (second_space_position-first_space_position-1 ));
        impl_->http_version = request_line.substr(second_space_position+1);
        
        return true;
    }

    bool Message::Request::parse_uri(const std::string& Uri)
    {
        if(impl_->uri == nullptr)
        {
            return false;
        }
        
        if(!impl_->uri->parse_from_string(Uri))
        {
            return false;
        }
        
        if(impl_->uri->get_path_string() == "")
        {
            impl_->request_uri = "/";
        }
        else
        {
            impl_->request_uri = impl_->uri->get_path_string();
        }
    
        // parse request host header from uri if any
        impl_->headers_map.insert({"Host", impl_->uri->get_host()});
        return true;
    }

    bool Message::Request::set_method(const std::string method)
    {
        impl_->method = method;
        return true;
    }

    bool Message::Request::set_http_version(const std::string http_version)
    {
        if(http_version.empty())
        {
            return false;
        }
        impl_->http_version = http_version;
        return true;
    }

    bool Message::Request::set_user_agent(const std::string user_agent)
    {
        if(user_agent.empty())
        {
            return false;
        }
        impl_->headers_map.insert({"User-Agent", user_agent});
        return true;
    }

    std::string Message::Request::get_method()
    {
        return impl_->method;
    }

    std::string Message::Request::get_request_uri()
    {
        return impl_->request_uri;
    }

    std::string Message::Request::get_http_version()
    {
        return impl_->http_version;
    }

    std::string Message::Request::get_header(const std::string& header_name)
    {
        auto iterator = impl_->headers_map.find(header_name);
        if (iterator == impl_->headers_map.end())
        {
            return "";
        }
        return iterator->second.c_str();
    }

    std::string Message::Request::get_body()
    {
        return impl_->body;
    }

    std::string Message::Request::get_generated_request()
    {
        return impl_->raw_request;
    }

    std::string Message::Request::get_raw_request()
    {
        return impl_->raw_request;
    }

    std::string Message::Request::get_host()
    {
        return impl_->uri->get_host();
    }

    std::string Message::Request::get_port()
    {
        return std::to_string(impl_->uri->get_port());
    }
}