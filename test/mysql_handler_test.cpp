#include <gtest/gtest.h>
#include "mysql_handler.hpp"

TEST(mysql_handler_tests, connect_to_mysql_test)
{
    mysql_handler mysql;

    ASSERT_TRUE(mysql.connect_to_mysql(3306, "root", "Qwer12340.0"));
}


