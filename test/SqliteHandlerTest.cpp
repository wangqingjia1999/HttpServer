#include "SqliteHandler.hpp"

#include <gtest/gtest.h>
#include <algorithm>

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
     *          | audio_name | audio_path | audio_duration|
     *          -------------------------------------------
     */

    std::vector<ColumnInfo> expected_user_table
    {
        { "0", "user_name",  "varchar(15)" },
        { "1", "user_age",   "int"  },
        { "2", "user_email", "text" }
    };
    auto user_table_columns = sqlite_handler.get_columns("user_table");
    for(int i = 0; i < user_table_columns.size(); ++i)
    {
        for(int j = 0; j < 3; ++j)
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
        { "3", "audio_duration", "int"  }
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
}