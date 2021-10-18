#include "Worker.hpp"
#include "Logger.hpp"
#include "SqliteHandler.hpp"
#include "StatusHandler.hpp"
#include "UnixDomainHelper.hpp"

#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define get_request m_connection->get_request()
#define get_response m_connection->get_response()

namespace
{
	/**
	 * Epoll triggered events max size
	 */
	constexpr size_t EPOLL_TRIGGERED_EVENTS_MAX_SIZE = 1024;

	/**
	 * Epoll interest/event list size
	 */
	constexpr size_t EPOLL_INTEREST_LIST_SIZE = 1024;

	/**
	 * Maximum socket listening buffer size in byte
	 */
	constexpr size_t MAXIMUM_LISTENING_PENDING_QUEUE = 4096;

	/**
	 * Maximum sending/receiving buffer size in byte
	 */
	constexpr size_t MAXIMUM_BUFFER_SIZE = 8192;
} // namespace

Worker::Worker(const int worker_socket)
    : m_epfd{-1}
    , m_worker_socket{worker_socket}
    , m_worker_socket_handler{new WorkerSocket()}
    , m_connection{std::make_shared<HTTP::Connection>()}
    , m_resource_handler{new SqliteHandler()}
    , m_server_socket{new WorkerSocket()}
{
	m_epfd = epoll_create(EPOLL_INTEREST_LIST_SIZE);
	if (m_epfd == -1)
	{
		Logger::error("worker epoll_create() error", errno);
		throw std::runtime_error("worker epoll_create() error");
	}

	epoll_event worker_socket_event;
	worker_socket_event.data.fd = m_worker_socket;
	worker_socket_event.events = EPOLLIN;
	if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_worker_socket,
	              &worker_socket_event) == -1)
	{
		Logger::error("worker epoll add error", errno);
		throw std::runtime_error("worker epoll add error");
	}
}

Worker::~Worker() { close(m_epfd); }

void Worker::event_loop()
{
	int sum = 0;
	struct epoll_event triggered_events[EPOLL_TRIGGERED_EVENTS_MAX_SIZE] = {0};

	for (;;)
	{
		switch (sum = epoll_wait(m_epfd, triggered_events,
		                         EPOLL_TRIGGERED_EVENTS_MAX_SIZE, -1))
		{
		case -1:
		{
			if (errno != EINTR)
				Logger::error("worker epoll_wait() error", errno);
			continue;
		}

		default:
		{
			for (int i = 0; i < sum; ++i)
			{
				int triggered_fd = triggered_events[i].data.fd;
				uint32_t triggered_event = triggered_events[i].events;

				if (triggered_event & EPOLLRDHUP)
				{
					epoll_ctl(m_epfd, EPOLL_CTL_DEL, triggered_fd, nullptr);
					close(triggered_fd);
					continue;
				}

				// receive master's work dispatch
				if ((triggered_event & EPOLLIN) &&
				    (triggered_fd == m_worker_socket))
				{
					int accepted_socket =
					    UnixDomainHelper::read_fd(m_worker_socket);
					if (accepted_socket > 0)
					{
						epoll_event new_client_event;
						new_client_event.data.fd = accepted_socket;
						new_client_event.events =
						    EPOLLIN | EPOLLET | EPOLLRDHUP;

						if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, accepted_socket,
						              &new_client_event) == -1)
						{
							Logger::error("worker epoll add error", errno);
							close(accepted_socket);
						}
					}
				}

				// accepted fd is readable
				if ((triggered_event & EPOLLIN) &&
				    (triggered_fd != m_worker_socket))
				{
					if (m_worker_socket_handler->read_from(triggered_fd))
					{
						request_core_handler(m_worker_socket_handler
						                         ->get_receive_buffer_string());

						m_server_socket->write_to(
						    triggered_fd, get_response->generate_response());

						get_request->clear_up();
						get_response->clear_up();
					}
					else
					{
						epoll_ctl(m_epfd, EPOLL_CTL_DEL, triggered_fd, nullptr);
					}

					continue;
				}

				if ((triggered_fd & EPOLLERR) &&
				    (triggered_fd != m_worker_socket))
				{
					Logger::debug("worker fd error", errno);
					close(triggered_fd);
				}
			}
		}
		}
	}
}

bool Worker::parse_request(const std::string& raw_request_string)
{
	get_request->set_raw_request(raw_request_string);
	return get_request->parse_raw_request();
}

std::string Worker::get_raw_request() { return get_request->get_raw_request(); }

std::string Worker::get_raw_response()
{
	return get_response->generate_response();
}

void Worker::set_raw_request(const std::string& raw_request)
{
	get_request->set_raw_request(raw_request);
}

bool Worker::handle_post_request()
{
	PercentEncoding percent_encoding;

	/**
	 * Append an additional '&', so that each name=value pair has a trailing '&'
	 */
	std::string body_buffer =
	    percent_encoding.decode(get_request->get_body()) + '&';

	while (!body_buffer.empty())
	{
		if (body_buffer.find_first_of('&') != std::string::npos)
		{
			if (body_buffer.find_first_of('=') != std::string::npos)
			{
				std::string name =
				    body_buffer.substr(0, body_buffer.find_first_of('='));

				std::string value =
				    body_buffer.substr(body_buffer.find_first_of('=') + 1,
				                       body_buffer.find_first_of('&') -
				                           body_buffer.find_first_of('=') - 1);

				post_data_map[std::move(name)] = std::move(value);

				body_buffer =
				    body_buffer.substr(body_buffer.find_first_of('&') + 1);
			}
		}
	}

	// TODO: decouple POST request processing logic from Worker

	return true;
}

void Worker::request_core_handler(const std::string& raw_request_string)
{
	if (!parse_request(raw_request_string))
	{
		Logger::error(
		    "worker parse request error with original request being: \n" +
		    raw_request_string);
		StatusHandler::handle_status_code(get_response, 400);
		return;
	}

	if (get_request->get_request_method() == "GET")
	{
		if (!m_resource_handler->fetch_resource(m_connection))
			StatusHandler::handle_status_code(get_response, 404);
		else
			StatusHandler::handle_status_code(get_response, 200);

		return;
	}
	else if (get_request->get_request_method() == "POST")
	{
		StatusHandler::handle_status_code(get_response, 501);
		return;
	}
	else if (get_request->get_request_method() == "PUT")
	{
		StatusHandler::handle_status_code(get_response, 501);
		return;
	}
	else if (get_request->get_request_method() == "HEAD")
	{
		StatusHandler::handle_status_code(get_response, 501);
		return;
	}
}