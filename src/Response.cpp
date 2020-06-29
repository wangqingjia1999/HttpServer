#include "Response.hpp"


namespace Message
{
	struct Message::Response::Impl
	{
		int statusCode;
		std::string protocolVersion = "HTTP/1.1";
		std::string reasonPhrase;
		std::map < std::string, std::string > headers;
		std::string body;
		std::streamoff bodyLength;
		std::string responseMessage;

		// only response has status code.
		std::map< int, std::string > statusCodeMap =
		{
			// 100-199: Informational status codes 
			{100,"Continue"},
			{101,"Switching Protocol"},
			{102,"Processing"},
			{103,"Early Hints"},

			// 200-299: Success status codes
			{200,"OK"},
			{201,"Created"},
			{202,"Accepted"},
			{203,"Non-Authoritative Information"},
			{204,"No Content"},
			{205,"Reset Content"},
			{206,"Partial Content"},
			{207,"Multi-Status"},
			{208,"Already Reported"},
			{226,"IM Used"},
			
			// 300-399: Redirection status codes
			{300,"Multiple Choice"},
			{301,"Moved Permanently"},
			{302,"Found"},
			{303,"See Other"},
			{304,"Not Modified"},
			{305,"Use Proxy"},
			{306,"unused"},
			{307,"Temporary Redirect"},
			{308,"Permanent Redirect"},

			// 400-499: Client error status codes
			{400,"Bad Request"},
			{401,"Unauthorized"},
			{402,"Payment Required"},
			{403,"Forbidden"},
			{404,"Not Found"},
			{405,"Method Not Allowed"},
			{406,"Not Acceptable"},
			{407,"Proxy Authentication Required"},
			{408,"Request Timeout"},
			{409,"Conflict"},
			{410,"Gone"},
			{411,"Length Required"},
			{412,"Precondition Failed"},
			{413,"Payload Too Large"},
			{414,"URI Too Long"},
			{415,"Unsupported Media Type"},
			{416,"Range Not Satisfiable"},
			{417,"Expectation Failed"},
			{418,"I'm a teapot"},
			{421,"Misdirected Request"},
			{422,"Unprocessable Entity"},
			{423,"Locked"},
			{424,"Failed Dependency"},
			{425,"Too Early"},
			{426,"Upgrade Required"},
			{428,"Precondition Required"},
			{429,"Too Many Requests"},
			{431,"Request Header Fields Too Large"},
			{451,"Unavailable For Legal Reasons"},
			
			// 500-599: Server error status codes
			{500,"Internal Server Error"},
			{501,"Not Implemented"},
			{502,"Bad Gateway"},
			{503,"Service Unavailable"},
			{504,"Gateway Timeout"},
			{505,"HTTP Version Not Supported"},
			{506,"Variant Also Negotiates"},
			{507,"Insufficient Storage"},
			{508,"Loop Detected"},
			{510,"Not Extended"},
			{511,"Network Authentication Required"},
		};

	};
	
	Message::Response::~Response() noexcept = default;

	Message::Response::Response() : impl_(new Impl) 
	{
	}

	Message::Response::Response(const Response& other) noexcept : impl_(new Impl) 
    {
        *this = other;
    }

	Response& Message::Response::operator=(const Response& other) noexcept
	{
		if(this != &other)
		{
			*impl_ = *other.impl_;
		}
		return *this;
	}

	Message::Response::Response(Response&& other) noexcept = default;
	Response& Message::Response::operator=(Response&& ) noexcept = default;

	int Message::Response::getStatusCode()
	{
		return impl_->statusCode;
	}

	std::string Message::Response::getProtocolVersion()
	{
		return impl_->protocolVersion;
	}

	std::string Message::Response::getReasonPhrase()
	{
		return impl_->reasonPhrase;
	}

	std::string Message::Response::getBody()
	{
		return impl_->body;
	}

	std::string Message::Response::getHeader(const std::string& headerName)
	{
		auto iterator = impl_->headers.find(headerName);
		if (iterator != impl_->headers.end())
		{
			return iterator->second.c_str();
		}
		else
		{
			return "";
		}
	}

	std::string Message::Response::getStatusCodeReasonString(const int statusCode)
	{
		return impl_->statusCodeMap[statusCode];
	}

	std::string Message::Response::getBodyLength()
	{
		return std::to_string(impl_->bodyLength);
	}

	int Message::Response::getBodyLengthInteger()
	{
		return impl_->bodyLength;
	}

	std::string Message::Response::getResponseMessage()
	{
		return impl_->responseMessage;
	}

	size_t Message::Response::getResponseMessageLength()
	{
		return impl_->responseMessage.size();
	}

	bool Message::Response::setStatus(int statusCodeInput)
	{
		impl_->statusCode = statusCodeInput;
		impl_->reasonPhrase = impl_->statusCodeMap[statusCodeInput];
		return true;
	}

	bool Message::Response::setProtocolVersion(const std::string versionProtocolInput)
	{
		impl_->protocolVersion = versionProtocolInput;
		return true;
	}
	
	bool Message::Response::setBody(const std::string& bodyInput)
	{
		if (!impl_->body.empty())
		{
			impl_->body.clear();
		}
		impl_->body = bodyInput;
		return true;
	}

	bool Message::Response::setBodyLength(const std::streamoff bodyLengthInput)
	{
		if (impl_->bodyLength != -1)
		{
			impl_->bodyLength = bodyLengthInput;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Message::Response::addHeader(const std::string& name, const std::string& value)
	{
		if (name.empty())
		{
			return false;
		}

		impl_->headers[name] = value;
		return true;
	}

	bool Message::Response::setContentLength(const std::streamoff& contentLengthInput)
	{
		if (contentLengthInput < 0)
		{
			return false;
		}
		else
		{
			addHeader("Content-Length", std::to_string(contentLengthInput));
			return true;
		}
	}

	bool Message::Response::setResponseMessage(const std::string& response)
	{
		impl_->responseMessage.clear();
		impl_->responseMessage = response;
		return true;
	}

	bool Message::Response::generateResponse()
	{
		std::ostringstream response;
		response << impl_->protocolVersion << " " << std::to_string(impl_->statusCode) << " " << impl_->reasonPhrase << "\r\n";

		for (auto position = impl_->headers.cbegin(); position != impl_->headers.cend(); ++position)
		{
			std::string name = position->first.c_str();
			std::string value = position->second.c_str();

			response << name << ": " << value << "\r\n";
		}

		if (!impl_->body.empty())
		{
			response << "\r\n" << impl_->body;
		}

		if (!setResponseMessage(response.str()))
		{
			return false;
		}
		return true;
	}

	bool Message::Response::readFile(const std::string& path)
	{
		// first, convert given relative path to absolute path
		std::string absolutePath;
		if (!convertPathToAbsolutePath(path, absolutePath))
		{
			setBodyLength(0);
			setBody("");
			return false;
		}

		std::ifstream file(absolutePath, std::ios_base::binary);

		if (!file.is_open())
		{
			setBodyLength(0);
			setBody("");
			return false;
		}

		// position at end of fileObject
		file.seekg(0, std::ios_base::end);

		auto size = file.tellg();

		if (!setContentLength(size))
		{
			setBodyLength(0);
			setBody("");
			return false;
		}

		if (!setBodyLength(size))
		{
			setBodyLength(0);
			setBody("");
			return false;
		}

		file.seekg(0);

		std::string resultOfRead((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		if (!setBody(resultOfRead.c_str()))
		{
			setBodyLength(0);
			setBody("");
			return false;
		}
		
		return true;
	}

	bool Message::Response::convertPathToAbsolutePath(const std::string& path,  std::string& absolutePath)
	{ 
	#ifdef __WIN32
		char buff[1000];
		_getcwd(buff, 1000);
		std::string currentWorkingDirectory = buff;

		auto projectPathEndPosition = currentWorkingDirectory.find("HttpServer");

		std::string pathWithForwardSlashes;

		for (char c : path)
		{
			if (c == '/')
			{
				pathWithForwardSlashes.push_back('\\');
			}
			else
			{
				pathWithForwardSlashes.push_back(c);
			}
		}

		absolutePath = currentWorkingDirectory.substr(0, projectPathEndPosition + 10) + "\\public_html" + pathWithForwardSlashes;

		return true;
	#endif // win32

	#ifdef __linux__
		char buffer[1024];
		if(getcwd(buffer, 1024) == nullptr)
		{
			return false;
		}

		std::string currentWorkingDirectory = buffer;

		auto projectPathEndPosition = currentWorkingDirectory.find("HttpServer");

		std::string pathWithForwardSlashes;

		for (char c : path)
		{
			if (c == '/')
			{
				pathWithForwardSlashes.push_back('\\');
			}
			else
			{
				pathWithForwardSlashes.push_back(c);
			}
		}

		absolutePath = currentWorkingDirectory.substr(0, projectPathEndPosition + 10) + "\\public_html" + pathWithForwardSlashes;
		return true;
	#endif // __LINUX__
	}

	bool Message::Response::setContentType(const std::string& path)
	{
		std::smatch matchResult;

		auto regex = std::regex("\\.([0-9a-zA-Z]+)$");

		std::string fileExtention;

		if (std::regex_search(path, matchResult, regex))
		{
			fileExtention = matchResult[1].str();
		}
		else
		{
			return false;
		}

		if ( fileExtention == "txt")
		{
			addHeader("Content-Type", "text/plain");
			return true;
		}
		else if (fileExtention == "html" || fileExtention == "htm")
		{
			addHeader("Content-Type", "text/html");
			return true;
		}
		else if (fileExtention == "css")
		{
			addHeader("Content-Type", "text/css");
			return true;
		}
		else if (fileExtention == "jpeg" || fileExtention == "jpg")
		{
			addHeader("Content-Type", "image/jpg");
			return true;
		}
		else if (fileExtention == "png")
		{
			addHeader("Content-Type", "image/png");
			return true;
		}
		else if (fileExtention == "gif")
		{
			addHeader("Content-Type", "image/gif");
			return true;
		}
		else if (fileExtention == "svg")
		{
			addHeader("Content-Type", "image/svg+xml");
			return true;
		}
		else if (fileExtention == "ico")
		{
			addHeader("Content-Type", "image/x-icon");
			return true;
		}
		else if (fileExtention == "json")
		{
			addHeader("Content-Type", "application/json");
			return true;
		}
		else if (fileExtention == "pdf")
		{
			addHeader("Content-Type", "application/pdf");
			return true;
		}
		else if (fileExtention == "js")
		{
			addHeader("Content-Type", "application/javascript");
			return true;
		}
		else if (fileExtention == "wasm")
		{
			addHeader("Content-Type", "application/wasm");
			return true;
		}
		else if (fileExtention == "xml")
		{
			addHeader("Content-Type", "application/xml");
			return true;
		}
		else if (fileExtention == "xhtml")
		{
			addHeader("Content-Type", "application/xhtml+xml");
			return true;
		}

		return false;
	}

	bool Message::Response::setContent(const std::string& path)
	{
		if (path.size() == 1 && path[0] == '/')
		{
			if(readFile("/index.html") && setContentType("/index.html"))
			{
				return true;
			}
		}

		// set body and body length if successful
		if (!readFile(path))
		{
			return false;
		}

		// set Content-Language
		// TODO: dynamicly generate language according to request
		impl_->headers.insert({ "Content-Language", "en-US" });

		// set content-type
		if (!setContentType(path))
		{
			return false;
		}

		return true;
	}

	bool Message::Response::hasHeader(const std::string& name)
	{
		auto iterator = impl_->headers.find(name);
		if (iterator == impl_->headers.end())
		{
			return false;
		}

		return true;
	}
}







