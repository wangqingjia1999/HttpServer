#include "MysqlHandler.hpp"

#include <gtest/gtest.h>
#include <iostream>

TEST(mysql_handler_tests, connect_to_mysql_test)
{
    MysqlHandler mysql;

    // port number can not be negative
    ASSERT_FALSE(mysql.connect_to_mysql(-3306, "bitate", "qwer"));
    // successfully connect to mysql based on given port, username, and password
    ASSERT_TRUE(mysql.connect_to_mysql(3306, "bitate", "qwer"));
}

