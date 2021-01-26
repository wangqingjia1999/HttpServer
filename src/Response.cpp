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
		: m_uri(std::make_shared<Uri> ()),
		  m_status_code(0),
		  m_protocol_version("HTTP/1.1"),
		  m_content_length(0)
	{
	}

	Message::Response::Response(const Response& other)
    {
		m_uri = std::make_shared< Uri > (*(other.m_uri));
		m_status_code = other.m_status_code;
		m_reason_phrase = other.m_reason_phrase;
		m_headers = other.m_headers;
		m_body = other.m_body;
		m_body_length = other.m_body_length;
		m_content_type = other.m_content_type;
		m_content_length = other.m_content_length;
		m_response_message = other.m_response_message;
    }

	Response& Message::Response::operator=(const Response& other)
	{
		if(this != &other)
		{
			m_uri = std::make_shared< Uri > (*(other.m_uri));
			m_status_code = other.m_status_code;
			m_reason_phrase = other.m_reason_phrase;
			m_headers = other.m_headers;
			m_body = other.m_body;
			m_body_length = other.m_body_length;
			m_content_type = other.m_content_type;
			m_content_length = other.m_content_length;
			m_response_message = other.m_response_message;
		}
		return *this;
	}

	int Message::Response::get_status_code()
	{
		return m_status_code;
	}

	std::string Message::Response::get_protocol_version()
	{
		return m_protocol_version;
	}

	std::string Message::Response::get_reason_phrase()
	{
		return m_reason_phrase;
	}

	std::string Message::Response::get_body()
	{
		return m_body;
	}

	std::string Message::Response::get_header(const std::string& header_name)
	{
		auto iterator = m_headers.find(header_name);
		if (iterator != m_headers.end())
		{
			return iterator->second.c_str();
		}
		else
		{
			return "";
		}
	}

	std::string Message::Response::get_status_code_reason_string(const int m_status_code)
	{
		return status_code_map[m_status_code];
	}

	std::string Message::Response::get_body_length()
	{
		m_body_length = m_body.size();
		return std::to_string(m_body_length);
	}

	std::string Message::Response::get_response_message()
	{
		return m_response_message;
	}

	size_t Message::Response::get_response_length()
	{
		return m_response_message.size();
	}

	std::string Message::Response::get_content_type()
	{
		return m_content_type;
	}

	bool Message::Response::set_status(int new_status_code)
	{
		m_status_code = new_status_code;
		m_reason_phrase = status_code_map[new_status_code];
		return true;
	}

	bool Message::Response::set_protocol_version(const std::string new_protocol_version)
	{
		m_protocol_version = new_protocol_version;
		return true;
	}
	
	bool Message::Response::set_body(const std::string& new_response_body)
	{
		if (!m_body.empty())
			m_body.clear();

		m_body = new_response_body;
		return true;
	}

	bool Message::Response::set_body_length(const std::streamoff new_body_length)
	{
		if (new_body_length != -1)
		{
			m_body_length = new_body_length;
			return true;
		}
		else
		{
			return false;
		}
	}

	void Message::Response::set_content_type(const std::string& new_content_type)
	{
		m_content_type = new_content_type;
		return;
	}

	bool Message::Response::add_header(const std::string& name, const std::string& value)
	{
		if (name.empty())
		{
			return false;
		}

		// insert new or update existing header
		m_headers[name] = value;
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
			m_content_length = new_content_length;
			add_header("Content-Length", std::to_string(new_content_length));
			return true;
		}
	}

	bool Message::Response::set_response_message(const std::string& new_response_message_string)
	{
		m_response_message.clear();
		m_response_message = std::move(new_response_message_string);
		return true;
	}

	bool Message::Response::set_reason_phrase(const int new_status_code)
	{
		auto header_position = status_code_map.find(new_status_code);
		if(header_position == status_code_map.cend())
		{
			return false;
		}

		m_reason_phrase = header_position->second;
		return true;
	}
	
	void Message::Response::generate_response()
	{
		std::ostringstream response;

		// Set first line of response string.
		response << m_protocol_version << " " << std::to_string(m_status_code) << " " << m_reason_phrase << "\r\n";

		// Iterate m_headers map and add to respoinse string.
		for (auto position = m_headers.cbegin(); position != m_headers.cend(); ++position)
		{
			std::string name = position->first.c_str();
			std::string value = position->second.c_str();

			response << name << ": " << value << "\r\n";
		}

		// Set m_body content.
		if (m_body.empty())
		{
			response << "\r\n";
		}
		else
		{
			response << "\r\n" << m_body;

			// Do not put '\r\n' at the end of the http message-m_body.
			// see: https://stackoverflow.com/a/13821352/11850070			
		}
		
		if (!set_response_message(response.str()))
		{
			// FIXME: logger "failed to set response message
			return;
		}
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
			m_content_type = "text/plain";
			return;
		}
		else if (file_extention == "html" || file_extention == "htm")
		{
			m_content_type = "text/html";
			return;
		}
		else if (file_extention == "css")
		{
			m_content_type = "text/css";
			return;
		}
		else if (file_extention == "jpeg" || file_extention == "jpg")
		{
			m_content_type = "image/jpg";
			return;
		}
		else if (file_extention == "png")
		{
			m_content_type = "image/png";
			return;
		}
		else if (file_extention == "gif")
		{
			m_content_type = "image/gif";
			return;
		}
		else if (file_extention == "svg")
		{
			m_content_type = "image/svg+xml";
			return;
		}
		else if (file_extention == "ico")
		{
			m_content_type = "image/x-icon";
			return;
		}
		else if (file_extention == "json")
		{
			m_content_type = "application/json";
			return;
		}
		else if (file_extention == "pdf")
		{
			m_content_type = "application/pdf";
			return;
		}
		else if (file_extention == "js")
		{
			m_content_type = "application/javascript";
			return;
		}
		else if (file_extention == "wasm")
		{
			m_content_type = "application/wasm";
			return;
		}
		else if (file_extention == "xml")
		{
			m_content_type = "application/xml";
			return;
		}
		else if (file_extention == "xhtml")
		{
			m_content_type = "application/xhtml+xml";
			return;
		}

		return;
	}

	bool Message::Response::set_content(const std::string& resource)
	{
		m_body = resource;
		return true;
	}

	bool Message::Response::has_header(const std::string& name)
	{
		return m_headers.find(name) != m_headers.cend();
	}

	void Message::Response::clear_up_header_fields()
	{
		m_headers.clear();
	}
	
}







