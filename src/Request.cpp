#include "URI.hpp"
#include "Request.hpp"

#include <sstream>

namespace Message
{
    struct Message::Request::Impl
    {
        std::shared_ptr< URI::URI > uri = std::make_shared< URI::URI > ();
        
        // Generated/Received raw request string
        std::string raw_request;

        // Request method. e.g. GET, POST
        std::string method;
        
        // default http_version
        std::string http_version = "HTTP/1.1";

        // Store the generated/received raw headers
        std::string headers;

        /**
         * For search bar: /?Search=This+is+just+a+demo
         * For normal get: /index.html
         * 
         * For more info, see: https://tools.ietf.org/html/rfc2616#section-5.1.2
         */
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

    Message::Request::Request(const Request& other)
    {
        *this = other;
    }
    Request& Message::Request::operator=(const Request& other)
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
    bool Message::Request::set_raw_request(std::string raw_request_string)
    {
        impl_->raw_request = raw_request_string;
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
        
        std::stringstream raw_request_string_stream;
        // set status line
        raw_request_string_stream << impl_->method << " " << impl_->uri->get_path_string() << " " << impl_->http_version << "\r\n";
        // set headers
        for(auto position = impl_->headers_map.cbegin(); position != impl_->headers_map.cend(); ++position)
        {
            raw_request_string_stream << position->first.c_str() << ": " << position->second.c_str() << "\r\n";    
        }
        // set headers end delimiter
        raw_request_string_stream << "\r\n";
        // set body 
        raw_request_string_stream << impl_->body;

        impl_->raw_request = raw_request_string_stream.str();
        return true;
    }

    bool Message::Request::parse_raw_request()
    {
        // parse request line (first line of request)
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

        // parse headers
        auto headers_end_delimiter = impl_->raw_request.find("\r\n\r\n");
        if (headers_end_delimiter == std::string::npos)
        {
            return false;
        }

        size_t headers_begin_position = request_line_end_delimiter + 2;
        /**
         * +2 is to make sure the last header also has the trailing "\r\n",
         * so that each header:value pair has "\r\n" at the end.
         * It makes the parsing of headers more easier.
         */
        size_t headers_end_position = headers_end_delimiter + 2;
        
        size_t headers_size = headers_end_position - headers_begin_position;
        
        impl_->headers = impl_->raw_request.substr(headers_begin_position, headers_size);
        if (!parse_headers(impl_->headers))
        {
            return false;
        }
        
        // set body
        impl_->body = impl_->raw_request.substr(headers_end_delimiter + 4);
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

                auto colon_position = single_header_line.find(':');

                if (colon_position == std::string::npos)
                {
                    impl_->headers.clear();
                    return false;
                }

                std::string header_name = single_header_line.substr(0, colon_position);
                std::string header_value = single_header_line.substr(colon_position + 1);

                /**
                 * Strip leading and trailing whitespaces of header name and header value;
                 * e.g. " Host  "        => "Host" 
                 *      "  bitate.com  " => "bitate.com"
                 * Note: Any number of spaces of tabs may exist around the ':'
                 */
                header_name = header_name.substr(header_name.find_first_not_of(' '), header_name.find_last_not_of(' ') + 1);
                header_value = header_value.substr(header_value.find_first_not_of(' '), header_value.find_last_not_of(' ') + 1);
            
                impl_->headers_map[header_name] = header_value;
                
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

    bool Message::Request::parse_uri(const std::string& URI)
    {
        if(impl_->uri == nullptr)
        {
            return false;
        }
        
        if(!impl_->uri->parse_from_string(URI))
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

    std::string Message::Request::get_request_method()
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

    bool Message::Request::has_header(const std::string& header_name) const
    {
        return impl_->headers_map.find(header_name) != impl_->headers_map.end();
    }

    bool Message::Request::has_http_version() const
    {
        return impl_->http_version != "";
    }

    bool Message::Request::has_method(const std::string& method) const
    {
        return impl_->method == method;
    }
}

