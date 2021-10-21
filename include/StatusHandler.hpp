#pragma once

#include <memory>

#include "Response.hpp"
#include "Timer.hpp"

/**
 * Set status code, reason-phrase, and appropriate headers
 * to @b response based on the @b status_code.
 */
namespace StatusHandler
{
	/**
	 * Add appropriate fields to response message based on given status code.
	 *
	 * @param[in] response
	 * 		Pointer to response object.
	 *
	 * @param[in] status_code
	 * 		Status code integer.
	 *
	 * @param[in] variadic arguments
	 * 		Possible arguments of specific status code for the convenience of
	 * processing status.
	 */
	void handle_status_code(const std::shared_ptr<Message::Response>& response,
	                        int status_code,
	                        const std::string& additional_info = "");
} // namespace StatusHandler