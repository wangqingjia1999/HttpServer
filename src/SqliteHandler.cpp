#include "SqliteHandler.hpp"

SqliteHandler::SqliteHandler()
    : m_connection(nullptr),
      m_statement(nullptr)
{
    if(sqlite3_open("http-server.db", &m_connection) != SQLITE_OK)  
        printf("can not open database");

    int result = 0;
    std::string create_user_table
    {
        "CREATE TABLE IF NOT EXISTS user_table("
            "user_name varchar(15),"
            "user_age int,"
            "user_email text"
        ")"
    };
    if((result = sqlite3_prepare_v2(m_connection, create_user_table.c_str(), -1, &m_statement, nullptr)) != SQLITE_OK)
        printf("can not prepare statement: %d, %s\n", result, sqlite3_errmsg(m_connection));
    
    if((result = sqlite3_step(m_statement)) != SQLITE_DONE)
        printf("can not execute statement: %d, %s\n", result, sqlite3_errmsg(m_connection));

    sqlite3_reset(m_statement);

    std::string create_audio_table
    {
        "CREATE TABLE IF NOT EXISTS audio_table("
            "audio_name text,"
            "audio_caption text,"
            "audio_path text,"
            "audio_duration int"
        ")"
    };
    if((result = sqlite3_prepare_v2(m_connection, create_audio_table.c_str(), -1, &m_statement, nullptr)) != SQLITE_OK)
        printf("can not prepare statement: %d, %s\n", result, sqlite3_errmsg(m_connection));
    
    if((result = sqlite3_step(m_statement)) != SQLITE_DONE)
        printf("can not execute statement: %d, %s\n", result, sqlite3_errmsg(m_connection));
    
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
    if((result = sqlite3_prepare_v2(m_connection, statement.c_str(), -1, &m_statement, nullptr)) != SQLITE_OK)
    {
        printf("can not prepare statement: %d, %s\n", result, sqlite3_errmsg(m_connection));
        sqlite3_reset(m_statement);
        return false;
    }
        
    int index = sqlite3_bind_parameter_index(m_statement, ":target_table_name");
    if((result = sqlite3_bind_text(m_statement, index, table_name.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK)
    {
        printf("can not bind value to statement: %d, %s\n", result, sqlite3_errmsg(m_connection));
        sqlite3_reset(m_statement);
        return false;
    }

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
    int result = 0;
    if((result = sqlite3_prepare_v2(m_connection, statement.c_str(), -1, &m_statement, nullptr)) != SQLITE_OK)
    {
        printf("can not prepare statement: %d, %s\n", result, sqlite3_errmsg(m_connection));
        sqlite3_reset(m_statement);
        return {};
    }
    
    std::vector<ColumnInfo> columns;
    while((result = sqlite3_step(m_statement)) == SQLITE_ROW)
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
