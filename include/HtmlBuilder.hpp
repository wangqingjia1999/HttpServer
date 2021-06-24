#pragma once

#include "SqliteHandler.hpp"

#include <string>

class HtmlBuilder
{
public:
    /**
     * @param[in]   titles
     *      std::vector<TitleEntry>
     * @param[in]   keyword
     *      keyword in the form of std::string
     * @return  
     *      html string with keyword highlighted in the form of std::string.
     */
    std::string highlight_keyword(const TitleEntry& titles, const std::string& keyword) const;
};