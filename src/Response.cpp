#include "Response.hpp"
#include "logger.hpp"
#include "mysql_handler.hpp"

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
} 


namespace Message
{
	struct Message::Response::Impl
	{	
		// Status code of response
		int status_code = 0;

		// Default protocol version
		std::string protocol_version = "HTTP/1.1";
		
		// Reason phrase for specific status code.
		std::string reason_phrase;
		
		// Header fields
		std::map < std::string, std::string > headers;
		
		// Body of response message
		std::string body;
		
		// Length of body of response message
		std::streamoff body_length;

		// Generated response message in the form of string
		std::string response_message;

		Uri::Uri uri;
	};
	
	Message::Response::~Response() noexcept = default;

	Message::Response::Response() : impl_(new Impl) 
	{
	}

	Message::Response::Response(const Response& other)
    {
        *this = other;
    }

	Response& Message::Response::operator=(const Response& other)
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

	std::string Message::Response::get_status_code_reason_string(const int status_code)
	{
		return status_code_map[status_code];
	}

	std::string Message::Response::get_body_length()
	{
		return std::to_string(impl_->body_length);
	}

	std::string Message::Response::get_response_message()
	{
		return impl_->response_message;
	}

	size_t Message::Response::get_response_length()
	{
		return impl_->response_message.size();
	}

	bool Message::Response::set_status(int status_code)
	{
		impl_->status_code = status_code;
		impl_->reason_phrase = status_code_map[status_code];
		return true;
	}

	bool Message::Response::set_protocol_version(const std::string protocol_version)
	{
		impl_->protocol_version = protocol_version;
		return true;
	}
	
	bool Message::Response::set_body(const std::string& response_body)
	{
		if (!impl_->body.empty())
		{
			impl_->body.clear();
		}
		impl_->body = response_body;
		return true;
	}

	bool Message::Response::set_body_length(const std::streamoff body_length)
	{
		if (impl_->body_length != -1)
		{
			impl_->body_length = body_length;
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

	bool Message::Response::set_content_length(const std::streamoff& content_length)
	{
		if (content_length < 0)
		{
			return false;
		}
		else
		{
			add_header("Content-Length", std::to_string(content_length));
			return true;
		}
	}

	bool Message::Response::set_response_message(const std::string& response)
	{
		impl_->response_message.clear();
		impl_->response_message = std::move(response);
		return true;
	}

	bool Message::Response::set_reason_phrase(const int status_code)
	{
		auto header_position = status_code_map.find(status_code);
		if(header_position == status_code_map.cend())
		{
			return false;
		}

		impl_->reason_phrase = header_position->second;
		return true;
	}
	
	bool Message::Response::generate_response()
	{
		std::ostringstream response;
		// Set first line of response string.
		response << impl_->protocol_version << " " << std::to_string(impl_->status_code) << " " << impl_->reason_phrase << "\r\n";

		// Iterate headers map and add to respoinse string.
		for (auto position = impl_->headers.cbegin(); position != impl_->headers.cend(); ++position)
		{
			std::string name = position->first.c_str();
			std::string value = position->second.c_str();

			response << name << ": " << value << "\r\n";
		}

		// Set body content.
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

		// if relative path only has '/', redirect it to index.html
		if( (relative_path[0] == '/')  &&  (relative_path.size() == 1) )
		{
			absolute_path = current_working_directory + "/public_html/index.html"; 
			return true;
		}

		absolute_path = current_working_directory + "/public_html" + relative_path;
		
		return true;
	}

	bool Message::Response::set_content_type(const std::string& request_uri)
	{
		std::smatch matchResult;

		auto regex = std::regex("\\.([0-9a-zA-Z]+)$");

		std::string fileExtention;

		if (std::regex_search(request_uri, matchResult, regex))
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

	bool Message::Response::set_content(const std::string& request_uri)
	{
		if (request_uri.size() == 1 && request_uri[0] == '/')
		{
			if(read_file("/index.html") && set_content_type("/index.html"))
			{
				return true;
			}
		}

		// Query request
		if(request_uri.find('?') != std::string::npos)
		{
			if(!impl_->uri.parse_from_string(request_uri))
			{
				return false;
			}
			if(impl_->uri.get_query().find_first_of('=') != std::string::npos)
			{
				std::string query_string = impl_->uri.get_query().substr( impl_->uri.get_query().find_first_of('=') + 1 );
				mysql_handler mysql;
				mysql.connect_to_mysql(3306, "bitate", "qwer");
				std::vector< std::string > query_result = mysql.fetch_user_by_name(query_string);
				
				if(query_result.empty())
				{	
					impl_->body = "<html><h1> Sorry, we can not find this guy. </h1>";
					impl_->body += "<a href=\"index.html\">Return to home</a></html>";
					impl_->body_length = impl_->body.size();
					return true;
				}

				std::string html_query_result = "<html><h1> Haha, We find this guy. </h1>";
				
				html_query_result += ("Name: " + query_result[0] + "<br>");
				html_query_result += ("Age: " + query_result[1] + "<br>");
				html_query_result += ("Email: " + query_result[2] + "<br>");
				html_query_result += ("Password: " + query_result[3] + "<br>");

				html_query_result += "<a href=\"index.html\">Return to home</a></html>";

				impl_->body = html_query_result;
				impl_->body_length = html_query_result.size();
				return true;
			}
			else
			{
				return false;
			}
		}

		// set body and body length
		if (!read_file(request_uri))
		{
			return false;
		}

		// set Content-Language
		// TODO: dynamicly generate language according to request
		impl_->headers.insert({ "Content-Language", "en-US" });

		// set content-type
		if (!set_content_type(request_uri))
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

	void Message::Response::clear_up_header_fields()
	{
		impl_->headers.clear();
	}
	
}







