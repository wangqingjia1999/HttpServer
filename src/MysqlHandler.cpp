#include "MysqlHandler.hpp"

MysqlHandler::MysqlHandler()
{
};

MysqlHandler::~MysqlHandler() = default;

bool MysqlHandler::initialize_mysql_layout()
{
    return true;
}

bool MysqlHandler::connect_to_mysql(const int port, const std::string username, const std::string password )
{
    

    return initialize_mysql_layout();
}

void MysqlHandler::execute_command(const std::string command_string)
{
}

void MysqlHandler::execute_query(const std::string query_string)
{
    
}

void MysqlHandler::create_table(const std::string table_info)
{
    
}

void MysqlHandler::drop_table(const std::string table_name)
{
    
}
