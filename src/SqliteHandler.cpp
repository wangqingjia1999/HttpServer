#include "SqliteHandler.hpp"

UserInfo::UserInfo()
{
}

UserInfo::UserInfo(std::string name, std::string password, std::string age, std::string email)
    : m_name(name), 
      m_password(password), 
      m_age(age), 
      m_email(email) 
{
}

SqliteHandler::SqliteHandler()
    : m_connection(nullptr),
      m_statement(nullptr)
{
    ServerConfiguration server_config;

    if(sqlite3_open(server_config.get_database_path().c_str(), &m_connection) != SQLITE_OK)  
        Logger::error("can not open database file with the path: " + server_config.get_database_path());

    std::string create_user_table
    {
        "CREATE TABLE IF NOT EXISTS user_table("
            "user_name varchar(15),"
            "user_password text,"
            "user_age int,"
            "user_email text"
        ")"
    };

    if(!prepare_statement(create_user_table))
    {
        Logger::error("Can create user table within database.");
    }
        
    int result = 0;
    if((result = sqlite3_step(m_statement)) != SQLITE_DONE)
        Logger::error("can not execute statement because " + std::string(sqlite3_errmsg(m_connection)));

    sqlite3_reset(m_statement);

    if((result = sqlite3_step(m_statement)) != SQLITE_DONE)
        Logger::error("can not execute statement because " + std::string(sqlite3_errmsg(m_connection)));
    
    sqlite3_reset(m_statement);
}

SqliteHandler::~SqliteHandler()
{
    if(m_statement)
        sqlite3_finalize(m_statement);
    if(m_connection)
        sqlite3_close(m_connection);
    
    sqlite3_shutdown();
}   

bool SqliteHandler::has_table(const std::string& table_name)
{
    std::string statement = "SELECT name FROM sqlite_master WHERE type='table' AND name=:target_table_name";
    int result = 0;

    if(!prepare_statement(statement))
        return false;

    if(!bind_text_data(":target_table_name", table_name))
        return false;

    if((result = sqlite3_step(m_statement)) == SQLITE_ROW)
    {
        if(*table_name.c_str() == *sqlite3_column_text(m_statement, 0))
        {
            sqlite3_reset(m_statement);
            return true;
        }
    }
    
    sqlite3_reset(m_statement);
    return false;
}

std::vector<ColumnInfo> SqliteHandler::get_columns(const std::string& table_name)
{
    std::string statement = "PRAGMA TABLE_INFO('" + table_name +  "')";

    if(!prepare_statement(statement))
        return {};
    
    std::vector<ColumnInfo> columns;
    while(sqlite3_step(m_statement) == SQLITE_ROW)
    {
        ColumnInfo single_conlumn_info;
        for(int i = 0; i < 3; ++i)
        {
            // Result row examples:
            // cid         name        type        notnull     dflt_value  pk        
            // ----------  ----------  ----------  ----------  ----------  ----------
            // 0           user_name   varchar(15) 0           0           0         
            // 1           user_age    int         0           0           0

            single_conlumn_info.push_back(
                std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_statement, i)))
            );
        }
        columns.push_back(single_conlumn_info);
    }
    
    sqlite3_reset(m_statement);
    return columns;
}

bool SqliteHandler::add_new_user(const UserInfo& user_info)
{
    std::string statement = "INSERT INTO user_table VALUES(:user_name, :user_password, :user_age, :user_email)";

    if(!prepare_statement(statement))
        return false;

    if(!bind_text_data(":user_name", user_info.m_name))
        return false;

    if(!bind_text_data(":user_password", user_info.m_password)) 
        return false;
    
    if(!bind_text_data(":user_age", user_info.m_age))
        return false;
    
    if(!bind_text_data(":user_email", user_info.m_email))
        return false;

    if(sqlite3_step(m_statement) != SQLITE_DONE)
    {
        sqlite3_reset(m_statement);
        return false;
    }

    sqlite3_reset(m_statement);
    return true;
}

std::vector<UserInfo> SqliteHandler::fetch_user_info(
    const std::string& user_name,
    const std::string& user_email
)
{
    if(user_name.empty() && user_email.empty())
        return {};

    std::string statement = "SELECT * FROM user_table ";

    if(!user_name.empty() && user_email.empty())
        statement += "WHERE user_name = :user_name";
    
    if(!user_email.empty() && user_name.empty())
        statement += "WHERE user_email = :user_email";
    
    if(!user_name.empty() && !user_email.empty())
        statement += "WHERE user_name = :user_name AND user_email = :user_email";

    if(!prepare_statement(statement))
        return {};
    
    if(!user_name.empty() && !bind_text_data(":user_name", user_name))
        return {};
    
    if(!user_email.empty() && !bind_text_data(":user_email", user_email))
        return {};

    std::vector<UserInfo> fetch_result;
    while(sqlite3_step(m_statement) == SQLITE_ROW)
    {
        UserInfo single_user_info;
        for(int i = 0; i < sqlite3_column_count(m_statement); ++i)
        {
            if(i == 0)
                single_user_info.m_name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_statement, i)));
            else if(i == 1)
                single_user_info.m_password = std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_statement, i)));
            else if(i == 2)
                single_user_info.m_age = std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_statement, i)));
            else if(i == 3)
                single_user_info.m_email = std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_statement, i)));
            else
                break;
        }
        fetch_result.push_back(single_user_info);
    }

    return fetch_result;
}

std::vector<UserInfo> SqliteHandler::fetch_user_info_by_name(const std::string& user_name)
{
    return fetch_user_info(user_name, "");
}

bool SqliteHandler::delete_user(const UserInfo& user_info)
{
    std::string statement 
    {
        "DELETE FROM user_table WHERE "
        "user_name = :user_name AND "
        "user_password = :user_password AND "
        "user_age = :user_age AND "
        "user_email = :user_email"
    };

    if(!prepare_statement(statement))
        return false;
    
    if(!bind_text_data(":user_name", user_info.m_name))
        return false;

    if(!bind_text_data(":user_password", user_info.m_password)) 
        return false;
    
    if(!bind_text_data(":user_age", user_info.m_age))
        return false;
    
    if(!bind_text_data(":user_email", user_info.m_email))
        return false;

    if(sqlite3_step(m_statement) != SQLITE_DONE)
    {
        sqlite3_reset(m_statement);
        return false;
    }

    sqlite3_reset(m_statement);
    return true;
}

bool SqliteHandler::prepare_statement(const std::string& statement)
{
    int result = 0;
    result = sqlite3_prepare_v2(m_connection, statement.c_str(), -1, &m_statement, nullptr);
    if(result != SQLITE_OK)
    {
        Logger::error("Error in preparing statement because " + std::string(sqlite3_errmsg(m_connection)));
        sqlite3_reset(m_statement);
        return false;
    }
    return  true;
}

bool SqliteHandler::bind_text_data(const std::string& placeholder, const std::string& data)
{   
    int result = 0;
    int placeholder_index = sqlite3_bind_parameter_index(m_statement, placeholder.c_str());
    if((result = sqlite3_bind_text(m_statement, placeholder_index, data.c_str(), -1, SQLITE_TRANSIENT)) != SQLITE_OK)
    {
        Logger::error("Error in binding data because " + std::string(sqlite3_errmsg(m_connection)));
        sqlite3_reset(m_statement);
        return false;
    }    
    return true;
}

std::vector<TitleEntry> SqliteHandler::search_news_title(const std::string& token)
{
    if(token.empty())
        return {};

    std::string statement = "SELECT * FROM abcnews_titles WHERE headline_text LIKE :token";

    if(!prepare_statement(statement))
    {
        Logger::error("cannot prepare statement for token: " + token);
        return {};
    }
        
    int result = 0;
    if((result = sqlite3_bind_text(m_statement, 1, ("% " + token + " %").c_str(), -1, SQLITE_TRANSIENT)) != SQLITE_OK)
    {
        Logger::error("Error in binding data because " + std::string(sqlite3_errmsg(m_connection)));
        sqlite3_reset(m_statement);
    }
    
    std::vector<TitleEntry> fetched_result;
    while(sqlite3_step(m_statement) == SQLITE_ROW)
    {
        TitleEntry title_entry;
        for(int i = 0; i < sqlite3_column_count(m_statement); ++i)
        {
            if(i == 0)
                title_entry.m_date = std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_statement, i)));
            else if(i == 1)
                title_entry.m_headline = std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_statement, i)));
            else if(i == 2)
                title_entry.m_publisher = std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_statement, i)));
            else
                break;
        }
        fetched_result.push_back(title_entry);
    }

    return fetched_result;
}