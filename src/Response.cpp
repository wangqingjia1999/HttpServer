#include "Response.hpp"
#include "Logger.hpp"
#include "Timer.hpp"

namespace
{
	// only response has status code.
	std::map<int, std::string> status_code_map = {
	    // 100-199: Informational status codes
	    {100, "Continue"},
	    {101, "Switching Protocol"},
	    {102, "Processing"},
	    {103, "Early Hints"},

	    // 200-299: Success status codes
	    {200, "OK"},
	    {201, "Created"},
	    {202, "Accepted"},
	    {203, "Non-Authoritative Information"},
	    {204, "No Content"},
	    {205, "Reset Content"},
	    {206, "Partial Content"},
	    {207, "Multi-Status"},
	    {208, "Already Reported"},
	    {226, "IM Used"},

	    // 300-399: Redirection status codes
	    {300, "Multiple Choice"},
	    {301, "Moved Permanently"},
	    {302, "Found"},
	    {303, "See Other"},
	    {304, "Not Modified"},
	    {305, "Use Proxy"},
	    {306, "unused"},
	    {307, "Temporary Redirect"},
	    {308, "Permanent Redirect"},

	    // 400-499: Client error status codes
	    {400, "Bad Request"},
	    {401, "Unauthorized"},
	    {402, "Payment Required"},
	    {403, "Forbidden"},
	    {404, "Not Found"},
	    {405, "Method Not Allowed"},
	    {406, "Not Acceptable"},
	    {407, "Proxy Authentication Required"},
	    {408, "Request Timeout"},
	    {409, "Conflict"},
	    {410, "Gone"},
	    {411, "Length Required"},
	    {412, "Precondition Failed"},
	    {413, "Payload Too Large"},
	    {414, "Uri Too Long"},
	    {415, "Unsupported Media Type"},
	    {416, "Range Not Satisfiable"},
	    {417, "Expectation Failed"},
	    {418, "I'm a teapot"},
	    {421, "Misdirected Request"},
	    {422, "Unprocessable Entity"},
	    {423, "Locked"},
	    {424, "Failed Dependency"},
	    {425, "Too Early"},
	    {426, "Upgrade Required"},
	    {428, "Precondition Required"},
	    {429, "Too Many Requests"},
	    {431, "Request Header Fields Too Large"},
	    {451, "Unavailable For Legal Reasons"},

	    // 500-599: Server error status codes
	    {500, "Internal Server Error"},
	    {501, "Not Implemented"},
	    {502, "Bad Gateway"},
	    {503, "Service Unavailable"},
	    {504, "Gateway Timeout"},
	    {505, "HTTP Version Not Supported"},
	    {506, "Variant Also Negotiates"},
	    {507, "Insufficient Storage"},
	    {508, "Loop Detected"},
	    {510, "Not Extended"},
	    {511, "Network Authentication Required"}};
} // namespace

namespace Message
{
	Message::Response::~Response() noexcept = default;

	Message::Response::Response()
	    : m_uri(std::make_shared<Uri>())
	    , m_status_code(0)
	    , m_protocol_version("HTTP/1.1")
	{
	}

	Message::Response::Response(const Response& other)
	{
		m_uri = std::make_shared<Uri>(*(other.m_uri));
		m_status_code = other.m_status_code;
		m_reason_phrase = other.m_reason_phrase;
		m_headers = other.m_headers;
		m_body = other.m_body;
		m_content_type = other.m_content_type;
	}

	Response& Message::Response::operator=(const Response& other)
	{
		if (this != &other)
		{
			m_uri = std::make_shared<Uri>(*(other.m_uri));
			m_status_code = other.m_status_code;
			m_reason_phrase = other.m_reason_phrase;
			m_headers = other.m_headers;
			m_body = other.m_body;
			m_content_type = other.m_content_type;
		}
		return *this;
	}

	int Message::Response::get_status_code() { return m_status_code; }

	std::string Message::Response::get_protocol_version()
	{
		return m_protocol_version;
	}

	std::string Message::Response::get_reason_phrase()
	{
		return m_reason_phrase;
	}

	std::string Message::Response::get_body() { return m_body; }

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

	std::string
	Message::Response::get_status_code_reason_string(const int status_code)
	{
		return status_code_map[status_code];
	}

	std::string Message::Response::get_content_type() { return m_content_type; }

	bool Message::Response::set_status(int status_code)
	{
		m_status_code = status_code;
		m_reason_phrase = status_code_map[status_code];
		return true;
	}

	bool
	Message::Response::set_protocol_version(const std::string protocol_version)
	{
		m_protocol_version = protocol_version;
		return true;
	}

	void Message::Response::set_body(const std::string& body)
	{
		m_body = body;
		add_header("Content-Length", std::to_string(m_body.size()));
	}

	void Message::Response::set_body(std::string&& body)
	{
		m_body = std::move(body);
		add_header("Content-Length", std::to_string(m_body.size()));
	}

	void Message::Response::set_content_type(const std::string& content_type)
	{
		m_content_type = content_type;
		add_header("Content-Type", m_content_type);
		return;
	}

	bool Message::Response::add_header(const std::string& name,
	                                   const std::string& value)
	{
		// insert new or update existing header
		m_headers[name] = value;
		return true;
	}

	bool Message::Response::set_reason_phrase(const int status_code)
	{
		auto header_position = status_code_map.find(status_code);
		if (header_position == status_code_map.cend())
		{
			return false;
		}

		m_reason_phrase = header_position->second;
		return true;
	}

	std::string Message::Response::generate_response()
	{
		std::ostringstream response;

		// Set first line of response string.
		response << m_protocol_version << " " << std::to_string(m_status_code)
		         << " " << m_reason_phrase << "\r\n";

		// Iterate m_headers map and add to respoinse string.
		for (auto position = m_headers.cbegin(); position != m_headers.cend();
		     ++position)
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

		return response.str();
	}

	bool Message::Response::has_header(const std::string& name)
	{
		return m_headers.find(name) != m_headers.cend();
	}

	void Message::Response::clear_up()
	{
		m_headers.clear();
		m_body.clear();
		m_content_type.clear();
		m_reason_phrase.clear();
	}

	std::string Message::Response::serialize_headers()
	{
		std::string serialized_headers_string;

		for (auto header : m_headers)
		{
			if (header.first == "Date" || header.first == "Host" ||
			    header.first == "Server")
				continue;

			serialized_headers_string +=
			    (header.first + ":" + header.second + "\n");
		}

		// custom end delimiter
		serialized_headers_string += "\n\n\n\n";

		return serialized_headers_string;
	}

	void Message::Response::deserialize(const std::string& message)
	{
		auto headers_end_delimiter = message.find("\n\n\n\n");

		if (headers_end_delimiter == std::string::npos)
			return;

		for (unsigned int i = 0; i < headers_end_delimiter; ++i)
		{
			unsigned int r = i;

			std::string header_key, header_value;

			while (r < headers_end_delimiter && message[r] != ':')
				header_key += message[r++];
			++r;
			while (r < headers_end_delimiter && message[r] != '\n')
				header_value += message[r++];

			i = r;

			add_header(header_key, header_value);
		}

		set_body(message.substr(headers_end_delimiter + 5));
	}
} // namespace Message
