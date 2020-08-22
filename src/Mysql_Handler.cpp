#include "Mysql_Handler.hpp"

struct Mysql_Handler::Impl
{
    // Pointer to connection object
    sql::Connection* connection_ptr = nullptr;
    // Pointer to query result set.
    sql::ResultSet* result_set = nullptr;
};

Mysql_Handler::Mysql_Handler() : impl_(new Impl) 
{
};
Mysql_Handler::~Mysql_Handler() = default;

Mysql_Handler::Mysql_Handler(const Mysql_Handler& other)
{   
    *this = other;
};
Mysql_Handler& Mysql_Handler::operator=(const Mysql_Handler& other)
{
    // prevent self-copy
    if(this != &other)
    {
        *impl_ = *other.impl_;
    }
    return *this;
};

Mysql_Handler::Mysql_Handler(Mysql_Handler&&) noexcept = default;
Mysql_Handler& Mysql_Handler::operator=(Mysql_Handler&& ) noexcept = default;

bool Mysql_Handler::initialize_mysql_layout()
{
    try
    {
        // create database for our project named Http_Server    
        execute_command("CREATE DATABASE IF NOT EXISTS Http_Server");
        // set utf-8 charset encoding
        execute_command("ALTER DATABASE Http_Server CHARACTER SET utf8 COLLATE utf8_general_ci");
        // create user information table
        std::string user_info_table = "CREATE TABLE IF NOT EXISTS Http_Server.user_info"
                                    "("
                                    // "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
                                    "user_name VARCHAR(20),"
                                    "user_age TINYINT UNSIGNED,"
                                    "user_email VARCHAR(255) NOT NULL PRIMARY KEY,"
                                    "user_password VARCHAR(255) NOT NULL"
                                    ")";
        execute_command(user_info_table);
        impl_->connection_ptr->setSchema("Http_Server");
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool Mysql_Handler::connect_to_mysql(const int port, const std::string username, const std::string password )
{
    sql::Driver *driver = get_driver_instance();
    if(port < 0)
    {
        return false;
    }
    const std::string mysql_connection_uri = "tcp://127.0.0.1:" + std::to_string(port);
    // Connect to mysql database.
    impl_->connection_ptr = driver->connect(mysql_connection_uri, username, password);
    return initialize_mysql_layout();
}

void Mysql_Handler::execute_command(const std::string command_string)
{
    sql::Statement* statement = impl_->connection_ptr->createStatement();
    statement->execute(command_string);    
    delete statement;
}

void Mysql_Handler::execute_query(const std::string query_string)
{
    sql::Statement* statement = impl_->connection_ptr->createStatement();
    impl_->result_set = statement->executeQuery(query_string);
    delete statement;
}

void Mysql_Handler::create_table(const std::string table_info)
{
    execute_command("CREATE TABLE IF NOT EXISTS " + table_info);
}

void Mysql_Handler::drop_table(const std::string table_name)
{
    execute_command("DROP TABLE IF EXISTS " + table_name);
}

bool Mysql_Handler::add_user(const std::string& name, const int age, const std::string& email, const std::string& password)
{
    sql::PreparedStatement* prepared_statement_ptr = impl_->connection_ptr->prepareStatement("INSERT IGNORE INTO user_info VALUES (?, ?, ?, ?)");
    prepared_statement_ptr->setString(1, name);
    prepared_statement_ptr->setInt(2, age);
    prepared_statement_ptr->setString(3, email);
    prepared_statement_ptr->setString(4, password);
    
    try
    {
        prepared_statement_ptr->execute();
        delete prepared_statement_ptr;
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

std::vector<std::string> Mysql_Handler::fetch_user_by_name(const std::string& query)
{
    try
    {
        sql::PreparedStatement* prepared_statement_ptr = impl_->connection_ptr->prepareStatement("SELECT * FROM Http_Server.user_info WHERE user_name=?");
        prepared_statement_ptr->setString(1,query);
        sql::ResultSet* result_set_ptr = prepared_statement_ptr->executeQuery();
        std::vector<std::string> query_result;
        if(result_set_ptr->next())
        {
            auto user_name = result_set_ptr->getString(1);
            query_result.push_back(user_name);
            auto user_age = result_set_ptr->getString(2);
            query_result.push_back(user_age);
            auto user_email = result_set_ptr->getString(3);
            query_result.push_back(user_email);
            auto user_password = result_set_ptr->getString(4);
            query_result.push_back(user_password);

            delete result_set_ptr;
            delete prepared_statement_ptr;    
            return query_result;
        }
        delete result_set_ptr;
        delete prepared_statement_ptr;
        return {};
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return {};
}