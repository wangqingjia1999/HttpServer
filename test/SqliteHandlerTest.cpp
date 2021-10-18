#include "SqliteHandler.hpp"

#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>

TEST(sqlite3_tests, has_table_test)
{
	SqliteHandler sqlite_handler;

	ASSERT_TRUE(sqlite_handler.has_table("user_table"));
}

TEST(sqlite3_tests, has_tables_test)
{
	SqliteHandler sqlite_handler;
	ASSERT_TRUE(sqlite_handler.has_table("news"));
	ASSERT_TRUE(sqlite_handler.has_table("news_index"));
}

TEST(sqlite3_tests, table_column_info_test)
{
	SqliteHandler sqlite_handler;
	/**
	 * Tables:
	 *      user_table:      User account info.
	 *          -------------------------------------
	 *          | user_name | user_age | user_email |
	 *          -------------------------------------
	 *      news:  news info (news body, news url, news publisher)
	 *          --------------------------
	 *          | body | url | publisher |
	 *          --------------------------
	 */

	std::vector<ColumnInfo> expected_user_table{
	    {"0", "user_name", "varchar(15)"},
	    {"1", "user_password", "text"},
	    {"2", "user_age", "int"},
	    {"3", "user_email", "text"}};
	auto user_table_columns = sqlite_handler.get_columns("user_table");
	for (int i = 0; i < user_table_columns.size(); ++i)
	{
		for (int j = 0; j < user_table_columns[i].size(); ++j)
		{
			ASSERT_EQ(user_table_columns[i][j], expected_user_table[i][j]);
		}
	}

	std::vector<ColumnInfo> expected_abcnews_titles_table{
	    {"0", "body", "int"}, {"1", "url", "text"}, {"2", "publisher", "text"}};
	auto abcnews_titles_table_columns = sqlite_handler.get_columns("news");
	for (int i = 0; i < abcnews_titles_table_columns.size(); ++i)
	{
		for (int j = 0; j < abcnews_titles_table_columns[i].size(); ++j)
		{
			ASSERT_EQ(user_table_columns[i][j], expected_user_table[i][j]);
		}
	}
}

TEST(sqlite3_tests, insert_into_test)
{
	SqliteHandler sqlite_handler;

	UserInfo user_info{"Tom", "1234567890", "20", "tom@gmail.com"};
	ASSERT_TRUE(sqlite_handler.add_new_user(user_info));
}

TEST(sqlite3_tests, delete_from_test)
{
	SqliteHandler sqlite_handler;

	UserInfo user_info{"Tom", "1234567890", "20", "tom@gmail.com"};

	ASSERT_TRUE(sqlite_handler.delete_user(user_info));
}

TEST(sqlite3_tests, fetch_user_info_test)
{
	SqliteHandler sqlite_handler;

	UserInfo user_info{"Denis", "1234567890", "20", "denis@gmail.com"};

	ASSERT_TRUE(sqlite_handler.add_new_user(user_info));

	auto fetch_by_user_name_result =
	    sqlite_handler.fetch_user_info("Denis", "");

	ASSERT_EQ(fetch_by_user_name_result[0].m_name, user_info.m_name);
	ASSERT_EQ(fetch_by_user_name_result[0].m_age, user_info.m_age);
	ASSERT_EQ(fetch_by_user_name_result[0].m_password, user_info.m_password);
	ASSERT_EQ(fetch_by_user_name_result[0].m_email, user_info.m_email);

	auto fetch_by_user_email_result =
	    sqlite_handler.fetch_user_info("", "denis@gmail.com");

	ASSERT_EQ(fetch_by_user_email_result[0].m_name, user_info.m_name);
	ASSERT_EQ(fetch_by_user_email_result[0].m_age, user_info.m_age);
	ASSERT_EQ(fetch_by_user_email_result[0].m_password, user_info.m_password);
	ASSERT_EQ(fetch_by_user_email_result[0].m_email, user_info.m_email);

	ASSERT_TRUE(sqlite_handler.delete_user(user_info));
}

TEST(sqlite3_tests, search_news_titles_test)
{
	SqliteHandler sqlite_handler;

	std::vector<Sentence> sentences = sqlite_handler.search_sentence("fly");

	EXPECT_EQ(sentences.size(), 100);
	EXPECT_TRUE(sentences[0].get_body().find("fly") != std::string::npos);
}