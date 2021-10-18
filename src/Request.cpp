#include "Request.hpp"
#include "Logger.hpp"
#include "Uri.hpp"

#include <sstream>

namespace Message
{
	Message::Request::Request()
	    : m_uri(std::make_shared<Uri>())
	    , m_http_version("HTTP/1.1")
	{
	}

	Message::Request::Request(const Request& other) { *this = other; }

	Message::Request::~Request() = default;

	Request& Message::Request::operator=(const Request& other) { return *this; }

	void Message::Request::set_raw_request(std::string raw_request_string)
	{
		m_raw_request = raw_request_string;
	}

	bool Message::Request::generate_request()
	{
		set_method();
		set_http_version();
		set_user_agent();

		std::stringstream raw_request_string_stream;
		// set status line
		raw_request_string_stream << m_method << " " << m_uri->get_path_string()
		                          << " " << m_http_version << "\r\n";
		// set m_headers
		for (auto position = m_headers_map.cbegin();
		     position != m_headers_map.cend(); ++position)
		{
			raw_request_string_stream << position->first.c_str() << ": "
			                          << position->second.c_str() << "\r\n";
		}
		// set m_headers end delimiter
		raw_request_string_stream << "\r\n";
		// set m_body
		raw_request_string_stream << m_body;

		m_raw_request = raw_request_string_stream.str();
		return true;
	}

	bool Message::Request::parse_raw_request()
	{
		// parse request line (first line of request)
		auto request_line_end_delimiter = m_raw_request.find("\r\n");
		if (request_line_end_delimiter == std::string::npos)
		{
			Logger::error("can't find CRLF in request line");
			return false;
		}

		std::string request_line =
		    m_raw_request.substr(0, request_line_end_delimiter);
		if (!parse_request_line(request_line))
		{
			Logger::error("can't parse request line: " + request_line);
			return false;
		}

		// parse request headers
		auto headers_end_delimiter = m_raw_request.find("\r\n\r\n");
		if (headers_end_delimiter == std::string::npos)
		{
			Logger::error("can't parse request headers: " + m_raw_request);
			return false;
		}

		size_t headers_begin_position = request_line_end_delimiter + 2;

		/**
		 * +2 is to make sure the last header also has the trailing "\r\n",
		 * so that each header:value pair has "\r\n" at the end.
		 * It makes the parsing of m_headers more easier.
		 */
		size_t headers_end_position = headers_end_delimiter + 2;

		size_t headers_size = headers_end_position - headers_begin_position;

		m_headers = m_raw_request.substr(headers_begin_position, headers_size);
		if (!parse_headers(m_headers))
		{
			Logger::info("Can not parse request headers.");
			return false;
		}

		// set m_body
		m_body = m_raw_request.substr(headers_end_delimiter + 4);
		return true;
	}

	bool Message::Request::parse_headers(const std::string& new_headers)
	{
		if (new_headers.find("\r\n") == std::string::npos)
		{
			m_headers.clear();
			return true;
		}

		std::string headers_buffer = m_headers;

		for (;;)
		{
			auto single_header_line_end_delimiter = headers_buffer.find("\r\n");

			// special case, only pass in "\r\n"
			if (headers_buffer == "\r\n")
			{
				m_headers.clear();
				return true;
			}

			if (single_header_line_end_delimiter != std::string::npos)
			{
				std::string single_header_line =
				    headers_buffer.substr(0, single_header_line_end_delimiter);

				auto colon_position = single_header_line.find(':');

				if (colon_position == std::string::npos)
				{
					m_headers.clear();
					return false;
				}

				std::string header_name =
				    single_header_line.substr(0, colon_position);
				std::string header_value =
				    single_header_line.substr(colon_position + 1);

				/**
				 * Strip leading and trailing whitespaces of header name and
				 * header value; e.g. " Host  "        => "Host" "  bitate.com
				 * " => "bitate.com" Note: Any number of spaces of tabs may
				 * exist around the ':'
				 */
				header_name =
				    header_name.substr(header_name.find_first_not_of(' '),
				                       header_name.find_last_not_of(' ') + 1);
				header_value =
				    header_value.substr(header_value.find_first_not_of(' '),
				                        header_value.find_last_not_of(' ') + 1);

				m_headers_map[header_name] = header_value;

				headers_buffer =
				    headers_buffer.substr(single_header_line_end_delimiter + 2);
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

		m_method = request_line.substr(0, first_space_position);

		m_request_uri = request_line.substr(
		    first_space_position + 1,
		    (second_space_position - first_space_position - 1));
		if (!parse_uri(m_request_uri))
		{
			m_request_uri.clear();
			m_method.clear();
			return false;
		}

		m_http_version = request_line.substr(second_space_position + 1);

		return true;
	}

	bool Message::Request::parse_uri(const std::string& uri)
	{
		if (!m_uri->parse_from_string(uri))
		{
			Logger::error("Request/cannot parse uri: " + uri);
			return false;
		}

		m_headers_map.insert({"Host", m_uri->get_host()});

		return true;
	}

	void Message::Request::set_method(const std::string new_method)
	{
		m_method = new_method;
	}

	void Message::Request::set_http_version(const std::string new_http_version)
	{
		m_http_version = new_http_version;
	}

	void Message::Request::set_user_agent(const std::string new_user_agent)
	{
		m_headers_map.insert({"User-Agent", new_user_agent});
	}

	std::string Message::Request::get_request_method() { return m_method; }

	std::string Message::Request::get_request_uri_string()
	{
		return m_request_uri;
	}

	std::shared_ptr<Uri> Message::Request::get_request_uri() { return m_uri; }

	std::string Message::Request::get_http_version() { return m_http_version; }

	std::string Message::Request::get_header(const std::string& header_name)
	{
		auto iterator = m_headers_map.find(header_name);
		if (iterator == m_headers_map.end())
		{
			return "";
		}
		return iterator->second.c_str();
	}

	std::string Message::Request::get_body() { return m_body; }

	std::string Message::Request::get_generated_request()
	{
		return m_raw_request;
	}

	std::string Message::Request::get_raw_request() { return m_raw_request; }

	std::string Message::Request::get_host() { return m_uri->get_host(); }

	std::string Message::Request::get_port()
	{
		return std::to_string(m_uri->get_port());
	}

	bool Message::Request::has_header(const std::string& header_name) const
	{
		return m_headers_map.find(header_name) != m_headers_map.end();
	}

	bool Message::Request::has_http_version() const
	{
		return m_http_version != "";
	}

	bool Message::Request::has_method(const std::string& m_method) const
	{
		return m_method == m_method;
	}

	bool Message::Request::has_query() const { return m_uri->has_query(); }

	void Message::Request::clear_up()
	{
		m_headers.clear();
		m_raw_request.clear();
		m_method.clear();
		m_headers_map.clear();
		m_body.clear();
	}
} // namespace Message
