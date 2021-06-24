#include "HtmlBuilder.hpp"

#include <gtest/gtest.h>

TEST(html_builder_tests, highlight_keyword_test)
{
    //TODO:

    HtmlBuilder html_builder;
    
    std::vector<TitleEntry> titles {
        { "20210624", "Everything You Wanted to Know About CAR and Were Too Embarrassed to Ask", "ABC" },
        { "20210624", "How To Find The Right CAR For Your Specific Product(Service).",           "ABC" },
        { "20210624", "Could This Report Be The Definitive Answer To Your CAR?",                 "ABC" },
        { "20210624", "Why CAR Is The Only Skill You Really Need",                               "ABC" }
    };

    std::vector<std::string> expected_result {
        "Everything You Wanted to Know About <mark>CAR</mark> and Were Too Embarrassed to Ask",
        "How To Find The Right <mark>CAR</mark> For Your Specific Product(Service).",
        "Could This Report Be The Definitive Answer To Your <mark>CAR</mark>?",
        "Why <mark>CAR</mark> Is The Only Skill You Really Need"
    };

    
}