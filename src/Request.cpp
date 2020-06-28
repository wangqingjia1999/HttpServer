#include "Uri.hpp"
#include "Request.hpp"

namespace Message
{
    struct Message::Request::Impl
    {
        std::shared_ptr< Uri::Uri > uri = std::shared_ptr< Uri::Uri >();

        std::string rawRequest;
    
        std::string method;
        std::string httpVersion;
        std::string headers;
        std::string requestUri;
        std::map< std::string, std::string> headersMap;
        std::string body = "";
    };

    Message::Request::~Request() = default;
    Message::Request::Request() : impl_(new Impl)
    {
    }

    // copy constructor
    Message::Request::Request(const Request& other) noexcept
    {
        *this = other;
    }
    // copy assignment operator
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

    bool Message::Request::setRawRequest(std::string rawRequestString)
    {
        impl_->rawRequest = rawRequestString;
        return true;
    }

    std::string Message::Request::generateRequest()
    {
        return "";
    }

    bool Message::Request::parseRawRequest()
    {
        // TODO: check whether the rawRequest is valid

        auto requestLineEndDelimiter = impl_->rawRequest.find("\r\n");
        if (requestLineEndDelimiter == std::string::npos)
        {
            return false;
        }

        std::string requestLine = impl_->rawRequest.substr(0, requestLineEndDelimiter);

        if (!parserequestLine(requestLine))
        {
            return false;
        }

        std::string rawRequestRemainder = impl_->rawRequest.substr(requestLineEndDelimiter + 2);

        auto headersEndDelimiter = rawRequestRemainder.find("\r\n\r\n");

        if (headersEndDelimiter == std::string::npos)
        {
            return false;
        }

        // +2 is to include the \r\n at the end of the last header
        impl_->headers = rawRequestRemainder.substr(0, headersEndDelimiter + 2);

        if (!parseHeaders(impl_->headers))
        {
            return false;
        }

        impl_->body = rawRequestRemainder.substr(headersEndDelimiter + 4);


        return true;
    }

    bool Message::Request::parseHeaders(const std::string& headers)
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

                
                std::string headerName = singleLine.substr(0, colonPosition);
                std::string headerValue = singleLine.substr(colonPosition + 1);

                // TODO: add new function to strip leading and trailing whitespace from string
                // Any number of spaces or tabs may be between the ":" and the value. 
                // Header lines beginning with space or tab are actually part of the previous header line,
                // folded into multiple lines for easy reading.
                if (headerName.find_last_of(' ') == headerName.size())
                {
                    headerName = headerName.substr(0, headerName.find_last_of(' '));
                }
                else if(headerName.find_first_of(' ') == 0)
                {
                    headerName = headerName.substr(1);
                }

                if (headerValue.find_first_of(' ') == 0)
                {
                    headerValue = headerValue.substr(1);
                }
                else if (headerValue.find_last_of(' ') == headerValue.size())
                {
                    headerValue = headerValue.substr(0, headerValue.find_last_of(' '));
                }
            
                impl_->headersMap[headerName] = headerValue;
                
                headersBuffer = headersBuffer.substr(singleLineEndDelimiter + 2);
            }
            else
            {
                break;
            }

        }

        return true;
    }

    bool Message::Request::parserequestLine(const std::string& requestLine)
    {
        if (requestLine.find(' ') == std::string::npos)
        {
            return false;
        }

        auto firstSpacePosition = requestLine.find_first_of(' ');
        auto secondSpacePositon = requestLine.find_last_of(' ');

        impl_->method = requestLine.substr(0, firstSpacePosition);
        impl_->requestUri = requestLine.substr(firstSpacePosition+1, (secondSpacePositon-firstSpacePosition-1 ));
        impl_->httpVersion = requestLine.substr(secondSpacePositon+1);
        
        return true;
    }

    std::string Message::Request::getMethod()
    {
        return impl_->method;
    }

    std::string Message::Request::getRequestUri()
    {
        return impl_->requestUri;
    }

    std::string Message::Request::getHttpVersion()
    {
        return impl_->httpVersion;
    }

    std::string Message::Request::getHeader(const std::string& headerName)
    {
        auto iterator = impl_->headersMap.find(headerName);
        if (iterator == impl_->headersMap.end())
        {
            return "";
        }
        return iterator->second.c_str();
    }

    std::string Message::Request::getBody()
    {
        return impl_->body;
    }
}