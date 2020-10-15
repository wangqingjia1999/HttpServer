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
    mysql_connection = mysql_init(nullptr);
    if( !mysql_connection ) 
        return false;

    if( !mysql_real_connect(mysql_connection, "localhost", username.c_str(), password.c_str(), nullptr, 0, nullptr, 0) )
    {
        mysql_close(mysql_connection);
        return false;
    }

    return initialize_mysql_layout();
}

void Mysql_Handler::execute_command(const std::string command_string)
{
}

void Mysql_Handler::execute_query(const std::string query_string)
{
    if( mysql_query(mysql_connection, query_string.c_str()) )
    {
        mysql_close(mysql_connection);
        exit(1);
    }
}

void Mysql_Handler::create_table(const std::string table_info)
{
    execute_command("CREATE TABLE IF NOT EXISTS " + table_info);
}

void Mysql_Handler::drop_table(const std::string table_name)
{
    execute_command("DROP TABLE IF EXISTS " + table_name);
}
