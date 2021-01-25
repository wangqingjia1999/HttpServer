#pragma once

#include <memory>
#include <vector>
#include <string>

struct Mysql_Layout;

/**
 * MySQL Connector/C++ Documentation
 * https://dev.mysql.com/doc/dev/connector-cpp/8.0/
 */
class MysqlHandler
{
public:
    MysqlHandler();
    ~MysqlHandler();

    MysqlHandler(const MysqlHandler&) = delete;
    MysqlHandler& operator=(const MysqlHandler&) = delete;
    
    MysqlHandler(MysqlHandler&&) = delete;
    MysqlHandler& operator=(MysqlHandler&&) = delete;

public:
    /**
     * Initialize database layout.
     * 
     * @return  
     *      True if successfully initialize mysql database and tables.
     */
    bool initialize_mysql_layout();

    /**
     * Initialize database layout based on given layout.
     * 
     * @param[in] layout  
     *      Layout of the database.
     * 
     * @return  
     *      True if successfully initialize mysql database and tables.
     */
    bool initialize_mysql_layout(Mysql_Layout& layout);

    /**
     * Connect to mysql based on given port, username and password.
     * 
     * @param[in] port  
     *      Port integer.
     * 
     * @param[in] username  
     *      Mysql username.
     * 
     * @param[in] password  
     *      Mysql password.
     * 
     * @return  
     *      True if successfully connects to Mysql.
     */
    bool connect_to_mysql(const int port, const std::string username, const std::string password );
    
    /**
     * Execute a command that does not return a result set.
     * 
     * @param[in] command_string  
     *      Command string.
     */
    void execute_command(const std::string command_string);

    /**
     * Execute a query.
     * 
     * @param[in] query_string  
     *      Query string. 
     */
    void execute_query(const std::string query_string);

    /**
     * Create a table.
     * 
     * @param[in] table_info 
     *      Table name and columns info.
     */
    void create_table(const std::string table_info);

    /**
     * Delete/Drop a table.
     * 
     * @param[in] table_name  
     *      Table name.
     */
    void drop_table(const std::string table_name);
};
