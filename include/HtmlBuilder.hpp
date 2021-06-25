#pragma once

#include "Sentence.hpp"
#include "SqliteHandler.hpp"

#include <string>

class HtmlBuilder
{
public:
    /**
     * @param[in]   sentence
     *      
     * @param[in]   keyword
     *      keyword in the form of std::string
     * @return  
     *      html string with keyword highlighted in the form of std::string.
     */
    std::string highlight_keyword(const Sentence& sentence, const std::string& keyword) const;
};