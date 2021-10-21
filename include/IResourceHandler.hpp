#pragma once

#include "Connection.hpp"

#include <memory>
#include <string>

/**
 * Interface for managing server side resources: mainly database.
 */
class IResourceHandler
{
public:
	/**
	 * Fetch resource(databse, files, etc) based on given connection's request
	 * and fill the resource into the connection's response.
	 *
	 * @return
	 *      True if succeeds.
	 */
	virtual bool
	fetch_resource(std::shared_ptr<HTTP::Connection> connection) = 0;

	IResourceHandler() noexcept = default;
	~IResourceHandler() = default;

	IResourceHandler(const IResourceHandler& other) = delete;
	IResourceHandler& operator=(const IResourceHandler& other) = delete;

	IResourceHandler(IResourceHandler&& other) = delete;
	IResourceHandler& operator=(IResourceHandler&& other) = delete;
};