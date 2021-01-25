#pragma once

#include <memory>

#include "Response.hpp"

namespace StatusHandler
{
    /**
	 * @brief  Handle given status code.
     * @param  response  Pointer to response object.
	 * @param  status_code  Status code integer.
	 */
	void handle_status_code( std::shared_ptr< Message::Response >& response, const int status_code);

}