#include <boost/test/unit_test.hpp>
#include "TextSearch.hpp"

struct TextSearchTestFixture
{
    TextSearch search;
    std::string text;
    void Search(std::string searchText)
    {
        search.SearchFor(searchText);
        for(char c : text)
            search.Handle(c);
    }
};

BOOST_FIXTURE_TEST_SUITE(TextSearchTest, TextSearchTestFixture)

BOOST_AUTO_TEST_CASE(Results_EmptyText_Empty)
{
    Search("abc");
    BOOST_CHECK(search.Results().empty());
}

BOOST_AUTO_TEST_CASE(Results_NoMatch_Empty)
{
    text = "Hello World";
    Search("abc");
    BOOST_CHECK(search.Results().empty());
}

BOOST_AUTO_TEST_CASE(Results_SingleMatch_ResultsContainsMatch)
{
    text = "Hello World";
    Search(" W");
    BOOST_REQUIRE_EQUAL(search.Results().size(), 1);
    BOOST_CHECK_EQUAL(search.Results().front().lineNumber, 1);
}

BOOST_AUTO_TEST_CASE(Results_SingleMatchInLine_ResultsContainsMatch)
{
    text = "<body>\nHello World\n</body>";
    Search(" W");
    BOOST_REQUIRE_EQUAL(search.Results().size(), 1);
    BOOST_CHECK_EQUAL(search.Results().front().lineNumber, 2);
}

BOOST_AUTO_TEST_CASE(Results_MatchesInDifferentLines_ResultsContainsMatches)
{
    text = "<body>\n<p>\nHello World\n</p>\nHello World!</body>";
    Search("World");
    BOOST_REQUIRE_EQUAL(search.Results().size(), 2);
    BOOST_CHECK_EQUAL(search.Results()[0].lineNumber, 3);
    BOOST_CHECK_EQUAL(search.Results()[1].lineNumber, 5);
}

BOOST_AUTO_TEST_SUITE_END()
