#include "mysql_handler.hpp"

#include <mysql_driver.h>
#include <mysql_connection.h>

struct mysql_handler::Impl
{
};

mysql_handler::mysql_handler() : impl_(new Impl) 
{
};
mysql_handler::~mysql_handler() = default;

mysql_handler::mysql_handler(const mysql_handler& other) noexcept
{   
    *this = other;
};
mysql_handler& mysql_handler::operator=(const mysql_handler& other) noexcept
{
    if(this != &other)
    {
        *impl_ = *other.impl_;
    }
    return *this;
};

mysql_handler::mysql_handler(mysql_handler&&) noexcept = default;
mysql_handler& mysql_handler::operator=(mysql_handler&& ) noexcept = default;

bool mysql_handler::connect_to_mysql(const int port, const std::string username, const std::string password )
{
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *connection;
    driver = sql::mysql::get_mysql_driver_instance();
    if(port < 0)
    {
        return false;
    }
    connection = driver->connect("tcp://127.0.0.1:3306", username, password);
    delete connection;
    return true;
}