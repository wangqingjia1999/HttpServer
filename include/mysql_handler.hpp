#ifndef MYSQL_HANDLER_HPP
#define MYSQL_HANDLER_HPP

#include <memory>
#include <vector>

// Including directly from cppconn/*.h 
// will speed up your build time
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

// tmp header
#include <iostream>

class mysql_handler
{
public:
    mysql_handler();
    ~mysql_handler();

    mysql_handler(const mysql_handler& );
    mysql_handler& operator=(const mysql_handler& );
    
    mysql_handler(mysql_handler&& ) noexcept;
    mysql_handler& operator=(mysql_handler&& ) noexcept;

public:

    /**
     * @brief  Initialize database layout.
     * @return  True if successfully initialize mysql database and tables.
     */
    bool initialize_mysql_layout();

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

    /**
     * @brief  Add new user into database.
     * @param  name  User name.
     * @param  age  User age.
     * @param  email  User email.
     * @return  True if successfully add user record.
     */
    bool add_user(const std::string& name, const int age, const std::string& email, const std::string& password);

    /**
     * @brief  Fetch user record by user name.
     * @param  query  User name.
     * @return  A record in the form of std:vector< std::string >.
     */
    std::vector< std::string > fetch_user_by_name(const std::string& query);
private:
    struct Impl;
    std::unique_ptr< Impl > impl_;
};

#endif
