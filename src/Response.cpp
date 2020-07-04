#include "Response.hpp"
#include <memory>

namespace Message
{
	struct Message::Response::Impl
	{	
		int status_code;
		std::string protocol_version = "HTTP/1.1";
		std::string reason_phrase;
		std::map < std::string, std::string > headers;
		std::string body;
		std::streamoff body_length;
		std::string response_message;

		// only response has status code.
		std::map< int, std::string > status_code_map =
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

	int Message::Response::get_status_code()
	{
		return impl_->status_code;
	}

	std::string Message::Response::get_protocol_version()
	{
		return impl_->protocol_version;
	}

	std::string Message::Response::get_reason_phrase()
	{
		return impl_->reason_phrase;
	}

	std::string Message::Response::get_body()
	{
		return impl_->body;
	}

	std::string Message::Response::get_header(const std::string& header_name)
	{
		auto iterator = impl_->headers.find(header_name);
		if (iterator != impl_->headers.end())
		{
			return iterator->second.c_str();
		}
		else
		{
			return "";
		}
	}

	std::string Message::Response::get_status_codeReasonString(const int status_code)
	{
		return impl_->status_code_map[status_code];
	}

	std::string Message::Response::get_body_length()
	{
		return std::to_string(impl_->body_length);
	}

	int Message::Response::get_body_lengthInteger()
	{
		return impl_->body_length;
	}

	std::string Message::Response::get_response_message()
	{
		return impl_->response_message;
	}

	size_t Message::Response::get_response_length()
	{
		return impl_->response_message.size();
	}

	bool Message::Response::set_status(int status_codeInput)
	{
		impl_->status_code = status_codeInput;
		impl_->reason_phrase = impl_->status_code_map[status_codeInput];
		return true;
	}

	bool Message::Response::set_protocol_version(const std::string versionProtocolInput)
	{
		impl_->protocol_version = versionProtocolInput;
		return true;
	}
	
	bool Message::Response::set_body(const std::string& bodyInput)
	{
		if (!impl_->body.empty())
		{
			impl_->body.clear();
		}
		impl_->body = bodyInput;
		return true;
	}

	bool Message::Response::set_body_length(const std::streamoff body_lengthInput)
	{
		if (impl_->body_length != -1)
		{
			impl_->body_length = body_lengthInput;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Message::Response::add_header(const std::string& name, const std::string& value)
	{
		if (name.empty())
		{
			return false;
		}

		// insert new or update existing header
		impl_->headers[name] = value;
		return true;
	}

	bool Message::Response::set_content_length(const std::streamoff& contentLengthInput)
	{
		if (contentLengthInput < 0)
		{
			return false;
		}
		else
		{
			add_header("Content-Length", std::to_string(contentLengthInput));
			return true;
		}
	}

	bool Message::Response::set_response_message(const std::string& response)
	{
		impl_->response_message.clear();
		impl_->response_message = response;
		return true;
	}

	bool Message::Response::set_reason_phrase(const int status_code)
	{
		auto header_position = impl_->status_code_map.find(status_code);
		if(header_position == impl_->status_code_map.cend())
		{
			return false;
		}

		impl_->reason_phrase = header_position->second;
		return true;
	}
	
	bool Message::Response::generate_response()
	{
		// assemble the response message
		std::ostringstream response;
		response << impl_->protocol_version << " " << std::to_string(impl_->status_code) << " " << impl_->reason_phrase << "\r\n";

		// iterate headers
		for (auto position = impl_->headers.cbegin(); position != impl_->headers.cend(); ++position)
		{
			std::string name = position->first.c_str();
			std::string value = position->second.c_str();

			response << name << ": " << value << "\r\n";
		}

		// set body
		if (impl_->body.empty())
		{
			response << "\r\n\r\n";
		}
		else
		{
			response << "\r\n" << impl_->body;

			auto body_end_delimiter_position = impl_->body.find("\r\n");
			if(body_end_delimiter_position == std::string::npos)
			{
				response << "\r\n";
			}
		}
		
		// store generated message to member variable
		if (!set_response_message(response.str()))
		{
			return false;
		}

		return true;
	}

	bool Message::Response::read_file(const std::string& path)
	{
		// first, convert given relative path to absolute path
		std::string absolutePath;
		if (!convert_path_to_absolute(path, absolutePath))
		{
			set_body_length(0);
			set_body("");
			return false;
		}

		std::ifstream file(absolutePath, std::ios_base::binary);

		if (!file.is_open())
		{
			set_body_length(0);
			set_body("");
			return false;
		}

		// position at end of fileObject
		file.seekg(0, std::ios_base::end);

		auto size = file.tellg();

		if (!set_content_length(size))
		{
			set_body_length(0);
			set_body("");
			return false;
		}

		if (!set_body_length(size))
		{
			set_body_length(0);
			set_body("");
			return false;
		}

		file.seekg(0);

		std::string resultOfRead((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		if (!set_body(resultOfRead.c_str()))
		{
			set_body_length(0);
			set_body("");
			return false;
		}
		
		return true;
	}

	bool Message::Response::convert_path_to_absolute(const std::string& path,  std::string& absolutePath)
	{ 
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
	}

	bool Message::Response::set_content_type(const std::string& path)
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
			add_header("Content-Type", "text/plain");
			return true;
		}
		else if (fileExtention == "html" || fileExtention == "htm")
		{
			add_header("Content-Type", "text/html");
			return true;
		}
		else if (fileExtention == "css")
		{
			add_header("Content-Type", "text/css");
			return true;
		}
		else if (fileExtention == "jpeg" || fileExtention == "jpg")
		{
			add_header("Content-Type", "image/jpg");
			return true;
		}
		else if (fileExtention == "png")
		{
			add_header("Content-Type", "image/png");
			return true;
		}
		else if (fileExtention == "gif")
		{
			add_header("Content-Type", "image/gif");
			return true;
		}
		else if (fileExtention == "svg")
		{
			add_header("Content-Type", "image/svg+xml");
			return true;
		}
		else if (fileExtention == "ico")
		{
			add_header("Content-Type", "image/x-icon");
			return true;
		}
		else if (fileExtention == "json")
		{
			add_header("Content-Type", "application/json");
			return true;
		}
		else if (fileExtention == "pdf")
		{
			add_header("Content-Type", "application/pdf");
			return true;
		}
		else if (fileExtention == "js")
		{
			add_header("Content-Type", "application/javascript");
			return true;
		}
		else if (fileExtention == "wasm")
		{
			add_header("Content-Type", "application/wasm");
			return true;
		}
		else if (fileExtention == "xml")
		{
			add_header("Content-Type", "application/xml");
			return true;
		}
		else if (fileExtention == "xhtml")
		{
			add_header("Content-Type", "application/xhtml+xml");
			return true;
		}

		return false;
	}

	bool Message::Response::set_content(const std::string& path)
	{
		if (path.size() == 1 && path[0] == '/')
		{
			if(read_file("/index.html") && set_content_type("/index.html"))
			{
				return true;
			}
		}

		// set body and body length if successful
		if (!read_file(path))
		{
			return false;
		}

		// set Content-Language
		// TODO: dynamicly generate language according to request
		impl_->headers.insert({ "Content-Language", "en-US" });

		// set content-type
		if (!set_content_type(path))
		{
			return false;
		}

		return true;
	}

	bool Message::Response::has_header(const std::string& name)
	{
		auto iterator = impl_->headers.find(name);
		if (iterator == impl_->headers.end())
		{
			return false;
		}

		return true;
	}

	void Message::Response::handle_status_code(const int status_code)
	{
		switch(status_code)
		{
			case 400:	// bad request
			{
				set_status(400);
				set_reason_phrase(400);
				add_header("Content-Type", "text/html");
				set_body("<html><h1> 400 Bad Request :) </h1></html>");
				break;
			}

			case 401:
			{
				
			}
		}

	}

}







