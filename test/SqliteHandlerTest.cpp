#include "SqliteHandler.hpp"

#include <gtest/gtest.h>
#include <algorithm>
#include <iostream>

TEST(sqlite_tests, has_table_test)
{
    SqliteHandler sqlite_handler;
    
    ASSERT_TRUE(sqlite_handler.has_table("user_table"));

    ASSERT_TRUE(sqlite_handler.has_table("audio_table"));
}

TEST(sqlite_tests, table_column_info_test)
{
    SqliteHandler sqlite_handler;
    /**
     * Tables:
     *      user_table:      User account info.
     *          -------------------------------------
     *          | user_name | user_age | user_email |
     *          -------------------------------------
     * 
     *      audio_table:  Resources details.
     *          -------------------------------------------
     *          | audio_name | audio_caption | audio_path |
     *          -------------------------------------------
     */

    std::vector<ColumnInfo> expected_user_table
    {
        { "0", "user_name",     "varchar(15)" },
        { "1", "user_password",    "text"     },
        { "2", "user_age",         "int"      },
        { "3", "user_email",       "text"     }
    };
    auto user_table_columns = sqlite_handler.get_columns("user_table");
    for(int i = 0; i < user_table_columns.size(); ++i)
    {
        for(int j = 0; j < user_table_columns[i].size(); ++j)
        {
            ASSERT_EQ(
                user_table_columns[i][j],
                expected_user_table[i][j]
            );
        }
    }

    std::vector< ColumnInfo > expected_audio_table
    {
        { "0", "audio_name",     "text" },
        { "1", "audio_caption",  "text" },
        { "2", "audio_path",     "text" },
    };
    auto audio_table_columns = sqlite_handler.get_columns("audio_table");
    for(int i = 0; i < audio_table_columns.size(); ++i)
    {
        for(int j = 0; j < audio_table_columns[i].size(); ++j)
        {
            EXPECT_EQ(
                audio_table_columns[i][j],
                expected_audio_table[i][j]
            );
        }       
    }
}

TEST(sqlite_tests, insert_into_test)
{
    SqliteHandler sqlite_handler;

    UserInfo user_info 
    {
        "Tom", "1234567890", "20", "tom@gmail.com"
    };
    ASSERT_TRUE(sqlite_handler.add_new_user(user_info));

    AudioInfo audio_info
    {
        "24-Dec-2020-13-09-03-3soLBPh71Y.mp3",
        "I'd have to throw myself in front of a train.",
        "/home/bitate/HttpServer/resource/audios/14-Dec-2020-15-06-39-1xVPfvJcrg.mp3",
    };
    ASSERT_TRUE(sqlite_handler.add_new_audio(audio_info));
}

TEST(sqlite_tests, delete_from_test)
{
    SqliteHandler sqlite_handler;

    UserInfo user_info 
    {
        "Tom", "1234567890", "20", "tom@gmail.com"
    };

    ASSERT_TRUE(sqlite_handler.delete_user(user_info));
}

TEST(sqlite_tests, fetch_user_info_test)
{
    SqliteHandler sqlite_handler;

    UserInfo user_info 
    {
        "Denis", "1234567890", "20", "denis@gmail.com"
    };

    ASSERT_TRUE(sqlite_handler.add_new_user(user_info));

    auto fetch_by_user_name_result = sqlite_handler.fetch_user_info("Denis", "");
    
    ASSERT_EQ(fetch_by_user_name_result[0].m_name, user_info.m_name);
    ASSERT_EQ(fetch_by_user_name_result[0].m_age, user_info.m_age);
    ASSERT_EQ(fetch_by_user_name_result[0].m_password, user_info.m_password);
    ASSERT_EQ(fetch_by_user_name_result[0].m_email, user_info.m_email);

    auto fetch_by_user_email_result = sqlite_handler.fetch_user_info("", "denis@gmail.com");
    
    ASSERT_EQ(fetch_by_user_email_result[0].m_name, user_info.m_name);
    ASSERT_EQ(fetch_by_user_email_result[0].m_age, user_info.m_age);
    ASSERT_EQ(fetch_by_user_email_result[0].m_password, user_info.m_password);
    ASSERT_EQ(fetch_by_user_email_result[0].m_email, user_info.m_email);

    ASSERT_TRUE(sqlite_handler.delete_user(user_info));
}