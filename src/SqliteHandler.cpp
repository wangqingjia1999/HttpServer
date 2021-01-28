#include "SqliteHandler.hpp"

SqliteHandler::SqliteHandler()
    : m_connection(nullptr),
      m_statement(nullptr)
{

}

SqliteHandler::~SqliteHandler()
{
    if(m_statement)
        sqlite3_finalize(m_statement);
    if(m_connection)
        sqlite3_close(m_connection);
    
    sqlite3_shutdown();
}   

bool SqliteHandler::execute(const std::string& statement)
{
    int return_value = 0;
    std::string db_name = "http-server.db";
    if((return_value = sqlite3_open(db_name.c_str(), &m_connection)) != SQLITE_OK)  
        printf("can not open database: %d, %s\n", return_value, sqlite3_errmsg(m_connection));
    
    if((return_value = sqlite3_prepare_v2(m_connection, statement.c_str(), -1, &m_statement, nullptr)) != SQLITE_OK)
        printf("can not prepare statement: %d, %s\n", return_value, sqlite3_errmsg(m_connection));

    
    
    while((return_value = return_value = sqlite3_step(m_statement)) == SQLITE_ROW)
    {
        for(int i = 0; i < sqlite3_column_count(m_statement); ++i)
        {
            printf("Value: %s\n", sqlite3_column_text(m_statement, i));
        }
        printf("-----------------------------------------------------------------\n");
    }

    return true;
}