#pragma once 

#include "Logger.hpp"

#include <sqlite3.h>
#include <string>
#include <vector>
#include <cstdio>

using ColumnInfo = std::vector<std::string>;

/**
 * This class maintains a Sqlite3 connection during its life-cycle.
 */
struct UserInfo
{
    std::string m_name;
    std::string m_password;
    std::string m_age;
    std::string m_email;

    UserInfo();
    UserInfo(std::string name, std::string password, std::string age, std::string email);
};

struct AudioInfo
{
    std::string m_name;
    std::string m_caption;
    std::string m_path;

    AudioInfo();
    AudioInfo(std::string name, std::string caption, std::string path);
};

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

    /**
     * Insert new user info record into database.
     * 
     * @param[in] user_info
     *      A new user's info.
     * 
     * @return 
     *      True if succeeds.
     */
    bool add_new_user(const UserInfo& user_info);

    /**
     * Insert new audio info record into database.
     * 
     * @param[in] audio_info
     *      A new audio's info.
     * 
     * @return 
     *      True if succeeds.
     */
    bool add_new_audio(const AudioInfo& audio_info);

    /**
     * Delete a user record from database.
     * 
     * @param[in] user_info
     *      User info record.
     * 
     * @return
     *      True if succeeds.
     */
    bool delete_user(const UserInfo& user_info);    

    /**
     * Fetch user record(s) which matches either user_name or user_email from database.
     * 
     * @param[in] user_name
     *      User name.
     * 
     * @param[in] user_email
     *      User email.
     * 
     * @return
     *      UserInfo(s).
     */
    std::vector<UserInfo> fetch_user_info(
        const std::string& user_name,
        const std::string& user_email
    );

    /**
     * Fetch user record(s) which matches the given user_name from database.
     * 
     * @param[in] user_name
     *      User name.
     * 
     * @return 
     *      UserInfo(s).
     */
    std::vector<UserInfo> fetch_user_info_by_name(const std::string& user_name);
    
private:
    /**
     * Prepare Sqlite3 statement.
     * 
     * @param[in] statement
     *      Statement to be prepared.
     * 
     * @return
     *      True if succeeds.
     */
    bool prepare_statement(const std::string& statement);

    /**
     * Replace placeholder with data with type of text.
     * 
     * @param[in] placeholder
     *      Sqlite3's parameter. Must begin with a ':'.
     * 
     * @param[in] text_data.
     *      Data with type of text to be bound.
     * 
     * @return
     *      True if succeeds.
     * 
     * @note
     *      The placeholders must be the colon-name-style parameters.
     *      e.g. :user_name, :user_email, 
     */
    bool bind_text_data(const std::string& placeholder, const std::string& text_data);

private:
    sqlite3* m_connection;
    sqlite3_stmt* m_statement;
};
