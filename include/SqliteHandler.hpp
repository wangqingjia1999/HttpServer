#pragma once 

#include <sqlite3.h>

#include <string>
#include <vector>

#include <stdio.h>

using ColumnInfo = std::vector<std::string>;

class SqliteHandler
{
public:
    SqliteHandler();
    ~SqliteHandler();

public:
    /**
     * Whether the table exists?
     * 
     * @param[in] table_name
     *      Target table name.
     * 
     * @return
     *      True if it already exists.
     */
    bool has_table(const std::string& table_name);

    /**
     * Get columns info of a table, which includes the column index
     * inside the table, the column name and the column type.
     *
     * @param[in] table_name
     *      Target table name.
     * 
     * @return
     *      All columns' info in the table.
     */
    std::vector<ColumnInfo> get_columns(const std::string& table_name);

private:
    sqlite3* m_connection;
    sqlite3_stmt* m_statement;
};
