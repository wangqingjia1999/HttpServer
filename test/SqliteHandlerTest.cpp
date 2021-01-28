#include "SqliteHandler.hpp"

#include <gtest/gtest.h>

TEST(sqlite_tests, connection_to_sqlite_test)
{
    SqliteHandler sqlite_handler;

    std::string s = "CREATE TABLE t1(num smallint, name varchar(10))";
    sqlite_handler.execute(s);

    std::string s1 = "INSERT INTO t1 VALUES(2, 'tom')";
    sqlite_handler.execute(s1);

    std::string s2 = "INSERT INTO t1 VALUES(21, 'jane')";
    sqlite_handler.execute(s2);

    std::string s3 = "INSERT INTO t1 VALUES(11, 'michael')";
    sqlite_handler.execute(s3);

    std::string select = "SELECT * FROM t1";
    sqlite_handler.execute(select);

}