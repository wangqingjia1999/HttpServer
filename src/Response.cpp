#include "Response.hpp"
#include "Logger.hpp"
#include "Timer.hpp"
#include "MysqlHandler.hpp"

#include <memory>


namespace
{
	// only response has status code.
	std::map< int, std::string > status_code_map = {
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
		{414,"Uri Too Long"},
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
} 

namespace Message
{	
	Message::Response::~Response() noexcept = default;

	Message::Response::Response()
		: uri(std::make_shared< Uri > ()),
		  status_code(0),
		  protocol_version("HTTP/1.1"),
		  content_length(0)
	{
	}

	Message::Response::Response(const Response& other)
    {
		uri = std::make_shared< Uri > (*(other.uri));
		status_code = other.status_code;
		reason_phrase = other.reason_phrase;
		headers = other.headers;
		body = other.body;
		body_length = other.body_length;
		content_type = other.content_type;
		content_length = other.content_length;
		response_message = other.response_message;
    }

	Response& Message::Response::operator=(const Response& other)
	{
		if(this != &other)
		{
			uri = std::make_shared< Uri > (*(other.uri));
			status_code = other.status_code;
			reason_phrase = other.reason_phrase;
			headers = other.headers;
			body = other.body;
			body_length = other.body_length;
			content_type = other.content_type;
			content_length = other.content_length;
			response_message = other.response_message;
		}
		return *this;
	}

	int Message::Response::get_status_code()
	{
		return status_code;
	}

	std::string Message::Response::get_protocol_version()
	{
		return protocol_version;
	}

	std::string Message::Response::get_reason_phrase()
	{
		return reason_phrase;
	}

	std::string Message::Response::get_body()
	{
		return body;
	}

	std::string Message::Response::get_header(const std::string& header_name)
	{
		auto iterator = headers.find(header_name);
		if (iterator != headers.end())
		{
			return iterator->second.c_str();
		}
		else
		{
			return "";
		}
	}

	std::string Message::Response::get_status_code_reason_string(const int status_code)
	{
		return status_code_map[status_code];
	}

	std::string Message::Response::get_body_length()
	{
		body_length = body.size();
		return std::to_string(body_length);
	}

	std::string Message::Response::get_response_message()
	{
		return response_message;
	}

	size_t Message::Response::get_response_length()
	{
		return response_message.size();
	}

	std::string Message::Response::get_content_type()
	{
		return content_type;
	}

	bool Message::Response::set_status(int new_status_code)
	{
		status_code = new_status_code;
		reason_phrase = status_code_map[new_status_code];
		return true;
	}

	bool Message::Response::set_protocol_version(const std::string new_protocol_version)
	{
		protocol_version = new_protocol_version;
		return true;
	}
	
	bool Message::Response::set_body(const std::string& new_response_body)
	{
		if (!body.empty())
			body.clear();

		body = new_response_body;
		return true;
	}

	bool Message::Response::set_body_length(const std::streamoff new_body_length)
	{
		if (new_body_length != -1)
		{
			body_length = new_body_length;
			return true;
		}
		else
		{
			return false;
		}
	}

	void Message::Response::set_content_type(const std::string& new_content_type)
	{
		content_type = new_content_type;
		return;
	}

	bool Message::Response::add_header(const std::string& name, const std::string& value)
	{
		if (name.empty())
		{
			return false;
		}

		// insert new or update existing header
		headers[name] = value;
		return true;
	}

	bool Message::Response::set_content_length(const size_t& new_content_length)
	{
		if (new_content_length < 0)
		{
			return false;
		}
		else
		{
			content_length = new_content_length;
			add_header("Content-Length", std::to_string(new_content_length));
			return true;
		}
	}

	bool Message::Response::set_response_message(const std::string& new_response_message_string)
	{
		response_message.clear();
		response_message = std::move(new_response_message_string);
		return true;
	}

	bool Message::Response::set_reason_phrase(const int new_status_code)
	{
		auto header_position = status_code_map.find(new_status_code);
		if(header_position == status_code_map.cend())
		{
			return false;
		}

		reason_phrase = header_position->second;
		return true;
	}
	
	void Message::Response::generate_response()
	{
		std::ostringstream response;

		// Set first line of response string.
		response << protocol_version << " " << std::to_string(status_code) << " " << reason_phrase << "\r\n";

		// Iterate headers map and add to respoinse string.
		for (auto position = headers.cbegin(); position != headers.cend(); ++position)
		{
			std::string name = position->first.c_str();
			std::string value = position->second.c_str();

			response << name << ": " << value << "\r\n";
		}

		// Set body content.
		if (body.empty())
		{
			response << "\r\n";
		}
		else
		{
			response << "\r\n" << body;

			// Do not put '\r\n' at the end of the http message-body.
			// see: https://stackoverflow.com/a/13821352/11850070			
		}
		
		if (!set_response_message(response.str()))
		{
			// FIXME: logger "failed to set response message
			return;
		}
	}

	bool Message::Response::read_file(const std::string& request_uri)
	{
		// first, convert given relative request_uri to absolute request_uri
		std::string absolute_path;
		if (!convert_path_to_absolute(request_uri, absolute_path))
		{
			set_body_length(0);
			set_body("");
			return false;
		}

		std::ifstream file(absolute_path, std::ios_base::binary);

		if (!file.is_open())
		{
			set_body_length(0);
			set_body("");
			return false;
		}

		// position at end of file object
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

		std::string read_result((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		if (!set_body(read_result.c_str()))
		{
			set_body_length(0);
			set_body("");
			return false;
		}
		
		return true;
	}

	bool Message::Response::convert_path_to_absolute(const std::string& relative_path,  std::string& absolute_path)
	{ 
		char path_buffer[1024];
		if(getcwd(path_buffer, 1024) == nullptr)
		{
			return false;
		}

		std::string current_working_directory = path_buffer;

		if(current_working_directory.find("HttpServer") == std::string::npos)
			return false;
		else // prevent the cwd path from containing '\\build\\test\\debug' after _getcwd() system call.
			current_working_directory = current_working_directory.substr( 0, current_working_directory.find("HttpServer") + 10 );

		// if relative path only has '/', redirect it to index.html
		if( (relative_path[0] == '/')  &&  (relative_path.size() == 1) )
		{
			absolute_path = current_working_directory + "/public_html/index.html"; 
			return true;
		}

		if( relative_path[0] == '/')
		{
			std::string relative_path_without_leading_forward_slash = relative_path.substr(1);
			absolute_path = current_working_directory + "/public_html/" + relative_path_without_leading_forward_slash;
		}
		else
		{
			absolute_path = current_working_directory + "/public_html/" + relative_path;
		}

		return true;
	}

	void Message::Response::parse_content_type(const std::string& request_uri)
	{
		std::smatch match_result;

		auto regex = std::regex("\\.([0-9a-zA-Z]+)$");

		std::string file_extention;

		if (std::regex_search(request_uri, match_result, regex))
		{
			file_extention = match_result[1].str();
		}
		else
		{
			return;
		}

		if ( file_extention == "txt")
		{
			content_type = "text/plain";
			return;
		}
		else if (file_extention == "html" || file_extention == "htm")
		{
			content_type = "text/html";
			return;
		}
		else if (file_extention == "css")
		{
			content_type = "text/css";
			return;
		}
		else if (file_extention == "jpeg" || file_extention == "jpg")
		{
			content_type = "image/jpg";
			return;
		}
		else if (file_extention == "png")
		{
			content_type = "image/png";
			return;
		}
		else if (file_extention == "gif")
		{
			content_type = "image/gif";
			return;
		}
		else if (file_extention == "svg")
		{
			content_type = "image/svg+xml";
			return;
		}
		else if (file_extention == "ico")
		{
			content_type = "image/x-icon";
			return;
		}
		else if (file_extention == "json")
		{
			content_type = "application/json";
			return;
		}
		else if (file_extention == "pdf")
		{
			content_type = "application/pdf";
			return;
		}
		else if (file_extention == "js")
		{
			content_type = "application/javascript";
			return;
		}
		else if (file_extention == "wasm")
		{
			content_type = "application/wasm";
			return;
		}
		else if (file_extention == "xml")
		{
			content_type = "application/xml";
			return;
		}
		else if (file_extention == "xhtml")
		{
			content_type = "application/xhtml+xml";
			return;
		}

		return;
	}

	bool Message::Response::set_content(const std::string& request_uri)
	{
		if (request_uri.size() == 1 && request_uri[0] == '/')
		{
			if(read_file("/index.html"))
			{
				parse_content_type("/index.html");
				return true;
			}
		}

		// Query request goes here:
		// ...

		
		// set body and body length
		if (!read_file(request_uri))
		{
			return false;
		}

		// set content-type
		parse_content_type(request_uri);

		return true;
	}

	bool Message::Response::has_header(const std::string& name)
	{
		auto iterator = headers.find(name);
		if (iterator == headers.end())
		{
			return false;
		}

		return true;
	}

	void Message::Response::clear_up_header_fields()
	{
		headers.clear();
	}
	
}







