#pragma once 

#include <sqlite3.h>
#include <memory>
#include <string>

#include <stdio.h>

class SqliteHandler
{
public:
    SqliteHandler();
    ~SqliteHandler();

public:
    bool execute(const std::string& statement);

private:
    sqlite3* m_connection;
    sqlite3_stmt* m_statement;
};


    //   sqlite3* pdb;
        
    //     const char* db_name = "http-server.db";

    //     int ret = 0;
    //     if((ret = sqlite3_open(db_name, &pdb)) != SQLITE_OK)
    //         printf("can not connect to: %d, %s\n", ret, sqlite3_errmsg(pdb));

    //     const char* s1 = "create table t2(number smallint, name varchar(10))";
    //     sqlite3_stmt* query = nullptr;
    //     if((ret = sqlite3_prepare_v2(pdb, s1, -1, &query, NULL)) != SQLITE_DONE);
    //         printf("can not prepare: %d, %s\n", ret, sqlite3_errmsg(pdb));

    //     if((ret = sqlite3_step(query)) != SQLITE_DONE)
    //         printf("can not execute: %d, %s", ret, sqlite3_errmsg(pdb));