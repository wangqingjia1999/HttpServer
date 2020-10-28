#include "Mysql_Handler.hpp"

Mysql_Handler::Mysql_Handler()
{
};

Mysql_Handler::~Mysql_Handler() = default;

bool Mysql_Handler::initialize_mysql_layout()
{
    return true;
}

bool Mysql_Handler::connect_to_mysql(const int port, const std::string username, const std::string password )
{
    

    return initialize_mysql_layout();
}

void Mysql_Handler::execute_command(const std::string command_string)
{
}

void Mysql_Handler::execute_query(const std::string query_string)
{
    
}

void Mysql_Handler::create_table(const std::string table_info)
{
    
}

void Mysql_Handler::drop_table(const std::string table_name)
{
    
}
