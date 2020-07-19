#include <gtest/gtest.h>
#include "mysql_handler.hpp"

TEST(mysql_handler_tests, connect_to_mysql_test)
{
    mysql_handler mysql;

    // port can not be negative
    ASSERT_FALSE(mysql.connect_to_mysql(-3306, "bitate", "qwer"));
    // successfully connect to mysql based on given port, username, and password
    ASSERT_TRUE(mysql.connect_to_mysql(3306, "bitate", "qwer"));
}

TEST(mysql_handler_tests, execute_commands_test)
{
    mysql_handler mysql;
    ASSERT_TRUE(mysql.connect_to_mysql(3306, "bitate", "qwer"));
    mysql.create_table("pet (name VARCHAR(20), owner VARCHAR(20))");
    mysql.drop_table("pet");
}


