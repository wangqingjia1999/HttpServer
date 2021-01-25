#pragma once

#include <memory>

#include "Response.hpp"

namespace StatusHandler
{
    /**
	 * Handle given status code.
	 * 
     * @param[in] response  
	 * 		Pointer to response object.
	 * 
	 * @param[in] status_code  
	 * 		Status code integer.
	 */
	void handle_status_code( std::shared_ptr< Message::Response >& response, const int status_code);

}