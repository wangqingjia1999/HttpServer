#include "mysql_handler.hpp"

struct mysql_handler::Impl
{
    // Pointer to connection object
    sql::Connection* connection_ptr = nullptr;
    // Pointer to query result set.
    sql::ResultSet* result_set = nullptr;
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
    sql::Driver *driver;
    // create a connection
    driver = get_driver_instance();
    if(port < 0)
    {
        return false;
    }
    const std::string mysql_connection_uri = "tcp://127.0.0.1:" + std::to_string(port);
    // Connect to mysql database.
    impl_->connection_ptr = driver->connect(mysql_connection_uri, username, password);
    // Connect to http_server database.
    impl_->connection_ptr->setSchema("http_server");
    return true;
}

void mysql_handler::execute_command(const std::string command_string)
{
    sql::Statement* statement = impl_->connection_ptr->createStatement();
    statement->execute(command_string);    
    delete statement;
}

void mysql_handler::execute_query(const std::string query_string)
{
    sql::Statement* statement = impl_->connection_ptr->createStatement();
    impl_->result_set = statement->executeQuery(query_string);
    delete statement;
}

void mysql_handler::create_table(const std::string table_info)
{
    execute_command("CREATE TABLE IF NOT EXISTS " + table_info);
}

void mysql_handler::drop_table(const std::string table_name)
{
    execute_command("DROP TABLE IF EXISTS " + table_name);
}
