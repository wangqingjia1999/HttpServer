#ifndef MYSQL_HANDLER_HPP
#define MYSQL_HANDLER_HPP

#include <memory>

// Including directly from cppconn/*.h 
// will speed up your build time
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>

// tmp header
#include <iostream>

class mysql_handler
{
public:
    mysql_handler();
    ~mysql_handler();

    mysql_handler(const mysql_handler& ) noexcept;
    mysql_handler& operator=(const mysql_handler& ) noexcept;
    
    mysql_handler(mysql_handler&& ) noexcept;
    mysql_handler& operator=(mysql_handler&& ) noexcept;

public:
    /**
     * @brief
     *      Connect to mysql based on given port, username and password.
     * @param[in]
     *      port: the port integer.
     * @param[in]
     *      username: Mysql database username.
     * @param[in]
     *      password: Mysql database password.
     * @return
     *      true if succeeds;
     *      false if fails.
     */
    bool connect_to_mysql(const int port, const std::string username, const std::string password );
    
    /**
     * @brief
     *      Execute a command that does not return a result set.
     * @param[in]
     *      command_string: the command string.
     */
    void execute_command(const std::string command_string);

    /**
     * @brief
     *      Execute a query.
     * @param[in]
     *      query_string: the query string. 
     */
    void execute_query(const std::string query_string);

    /**
     * @brief
     *      Create a table.
     * @param[in]
     *      table_info: table name and columns.
     */
    void create_table(const std::string table_info);

    /**
     * @brief
     *      Delete/Drop a table.
     * @param[in]  
     *      table_name: the name of table to be deleted.
     */
    void drop_table(const std::string table_name);

    bool insert_value();
    bool delete_value();


private:
    struct Impl;
    std::unique_ptr< Impl > impl_;
};

#endif