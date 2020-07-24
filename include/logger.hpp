#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>

class logger
{
public:
    logger();
    ~logger() noexcept;

    logger(const logger&);
    logger& operator=(const logger&);

    logger(logger&&) noexcept;
    logger& operator=(logger&&) noexcept;
public:
    /**
     * @brief
     *      Record the message.
     * @param[in]
     *      Message string.
     */
    static void record(const std::string& message_string);
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif