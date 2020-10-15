#ifndef MYSQL_HANDLER_HPP
#define MYSQL_HANDLER_HPP

#include <memory>
#include <vector>
#include <string>

#ifdef _WIN32
    #include <mysql.h>
#elif __linux__
    // For linux 
#endif

struct Mysql_Layout;

class Mysql_Handler
{
    // Life-cycle management
public:
    Mysql_Handler();
    ~Mysql_Handler();

    Mysql_Handler(const Mysql_Handler&) = delete;
    Mysql_Handler& operator=(const Mysql_Handler&) = delete;
    
    Mysql_Handler(Mysql_Handler&&) = delete;
    Mysql_Handler& operator=(Mysql_Handler&&) = delete;

public:
    /**
     * @brief  Initialize database layout.
     * @return  True if successfully initialize mysql database and tables.
     */
    bool initialize_mysql_layout();

    /**
     * @brief  Initialize database layout based on given layout.
     * @param  layout  Layout of the database.
     * @return  True if successfully initialize mysql database and tables.
     */
    bool initialize_mysql_layout(Mysql_Layout& layout);

    /**
     * @brief  Connect to mysql based on given port, username and password.
     * @param  port  Port integer.
     * @param  username  Mysql username.
     * @param  password  Mysql password.
     * @return  True if successfully connects to Mysql.
     */
    bool connect_to_mysql(const int port, const std::string username, const std::string password );
    
    /**
     * @brief  Execute a command that does not return a result set.
     * @param  command_string  Command string.
     */
    void execute_command(const std::string command_string);

    /**
     * @brief  Execute a query.
     * @param  query_string  Query string. 
     */
    void execute_query(const std::string query_string);

    /**
     * @brief  Create a table.
     * @param  table_info  Table name and columns info.
     */
    void create_table(const std::string table_info);

    /**
     * @brief  Delete/Drop a table.
     * @param  table_name  Table name.
     */
    void drop_table(const std::string table_name);

private:
    MYSQL* mysql_connection = nullptr;

};

struct Mysql_Layout
{


};

#endif
