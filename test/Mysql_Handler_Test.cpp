#include "Mysql_Handler.hpp"

#include <gtest/gtest.h>
#include <vector>

TEST(mysql_handler_tests, connect_to_mysql_test)
{
    Mysql_Handler mysql;

    // port can not be negative
    ASSERT_FALSE(mysql.connect_to_mysql(-3306, "bitate", "qwer"));
    // successfully connect to mysql based on given port, username, and password
    ASSERT_TRUE(mysql.connect_to_mysql(3306, "bitate", "qwer"));
}

TEST(mysql_handler_tests, initialize_mysql_layout_test)
{
    Mysql_Handler mysql;
    ASSERT_TRUE(mysql.connect_to_mysql(3306, "bitate", "qwer"));
    ASSERT_TRUE(mysql.initialize_mysql_layout());
}

TEST(mysql_handler_tests, add_new_user_test)
{
    Mysql_Handler mysql;
    ASSERT_TRUE(mysql.connect_to_mysql(3306, "bitate", "qwer"));
    ASSERT_TRUE(mysql.initialize_mysql_layout());

    struct single_user_record
    {
        std::string name;
        int age;
        std::string email;
        std::string password;
    };

    const std::vector<single_user_record> user_info_records = 
    {
        { "Tom",     20, "tom@gmail.com",   "123456789" },
        { "Jane",    23, "Jane@gmail.com",  "qwerqwer"  },
        { "Michael", 25, "Happy@gmail.com", "password"  }
    };

    size_t index = 0;
    for( const single_user_record& user_record : user_info_records)
    {
        ASSERT_TRUE(mysql.add_user(user_record.name, user_record.age, user_record.email, user_record.password)) << index;
        ++index;
    }
}

TEST(mysql_handler_tests, fetch_user_by_name_test)
{
    Mysql_Handler mysql;
    ASSERT_TRUE(mysql.connect_to_mysql(3306, "bitate", "qwer"));
    ASSERT_TRUE(mysql.initialize_mysql_layout());

    struct single_user_record
    {
        std::string name;
        int age;
        std::string email;
        std::string password;
    };
    const std::vector<single_user_record> user_info_records = 
    {
        { "Tom",     20, "tom@gmail.com",   "123456789" },
        { "Jane",    23, "Jane@gmail.com",  "qwerqwer"  },
        { "Michael", 25, "Happy@gmail.com", "password"  }
    };
    
    size_t index = 0;
    for( const single_user_record& user_record : user_info_records)
    {
        ASSERT_TRUE(mysql.add_user(user_record.name, user_record.age, user_record.email, user_record.password)) << index;
        ++index;
    }

    std::string query = "Michael";
    std::vector<std::string> query_result = { "Michael", "25", "Happy@gmail.com", "password" };
    ASSERT_EQ(mysql.fetch_user_by_name(query), query_result);
}

