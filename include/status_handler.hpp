#ifndef STATUS_HANDLER_HPP
#define STATUS_HANDLER_HPP

#include <memory>
#include "Response.hpp"

namespace status_handler
{
    /**
	 * Handle given status code.
	 * 
     * @param[in] response
     *      A pointer to response object.
	 * @param[in]
	 * 		Status code integer.
	 */
	void handle_status_code( std::shared_ptr< Message::Response >& response, const int status_code);

}
#endif