#ifndef MYSQL_HANDLER_HPP
#define MYSQL_HANDLER_HPP

#include <memory>

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
     */
    bool connect_to_mysql(const int port, const std::string username, const std::string password );
    
    bool create_table();
    bool insert_value();
    bool delete_value();
    bool delete_table();
    
private:
    struct Impl;
    std::unique_ptr< Impl > impl_;
};

#endif