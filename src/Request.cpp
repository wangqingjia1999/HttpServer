#include "Uri.hpp"
#include "Request.hpp"
#include <sstream>

namespace Message
{
    struct Message::Request::Impl
    {
        std::shared_ptr< Uri::Uri > uri = std::make_shared< Uri::Uri > ();
        
        std::string raw_request;
    
        std::string method;
        std::string http_version;
        std::string headers;
        std::string requestUri;
        std::map< std::string, std::string> headers_map;
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
        // BUG: 
        auto requestLineEndDelimiter = impl_->raw_request.find("\r\n");
        if (requestLineEndDelimiter == std::string::npos)
        {
            return false;
        }

        std::string requestLine = impl_->raw_request.substr(0, requestLineEndDelimiter);

        if (!parse_request_line(requestLine))
        {
            return false;
        }

        std::string rawRequestRemainder = impl_->raw_request.substr(requestLineEndDelimiter + 2);

        auto headersEndDelimiter = rawRequestRemainder.find("\r\n\r\n");

        if (headersEndDelimiter == std::string::npos)
        {
            return false;
        }

        // +2 is to include the \r\n at the end of the last header
        impl_->headers = rawRequestRemainder.substr(0, headersEndDelimiter + 2);

        if (!parse_headers(impl_->headers))
        {
            return false;
        }

        impl_->body = rawRequestRemainder.substr(headersEndDelimiter + 4);


        return true;
    }

    bool Message::Request::parse_headers(const std::string& headers)
    {
        if (headers.find("\r\n") == std::string::npos)
        {
            return false;
        }

        std::string headersBuffer = headers;

        
        for(;;)
        {
            auto singleLineEndDelimiter = headersBuffer.find("\r\n");

            if (singleLineEndDelimiter != std::string::npos)
            {
                std::string singleLine = headersBuffer.substr(0, singleLineEndDelimiter);

                auto colonPosition = singleLine.find(':');

                if (colonPosition == std::string::npos)
                {
                    return false;
                }

                
                std::string header_name = singleLine.substr(0, colonPosition);
                std::string headerValue = singleLine.substr(colonPosition + 1);

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
                
                headersBuffer = headersBuffer.substr(singleLineEndDelimiter + 2);
            }
            else
            {
                break;
            }

        }

        return true;
    }

    bool Message::Request::parse_request_line(const std::string& requestLine)
    {
        if (requestLine.find(' ') == std::string::npos)
        {
            return false;
        }

        auto firstSpacePosition = requestLine.find_first_of(' ');
        auto secondSpacePositon = requestLine.find_last_of(' ');

        impl_->method = requestLine.substr(0, firstSpacePosition);
        impl_->requestUri = requestLine.substr(firstSpacePosition+1, (secondSpacePositon-firstSpacePosition-1 ));
        impl_->http_version = requestLine.substr(secondSpacePositon+1);
        
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
            impl_->requestUri = "/";
        }
        else
        {
            impl_->requestUri = impl_->uri->get_path_string();
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
        return impl_->requestUri;
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