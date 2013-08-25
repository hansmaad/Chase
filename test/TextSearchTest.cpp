#include <boost/test/unit_test.hpp>
#include "TextSearch.hpp"

struct TextSearchTestFixture
{
    std::string text;

    std::vector<TextSearchMatch> Search(std::string searchText)
    {
        return SearchText(text, searchText);
    }
};

BOOST_FIXTURE_TEST_SUITE(TextSearchTest, TextSearchTestFixture)

BOOST_AUTO_TEST_CASE(Results_EmptyText_Empty)
{
    auto result = Search("abc");
    BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(Results_NoMatch_Empty)
{
    text = "Hello World";
    auto result = Search("abc");
    BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(Results_SingleMatch_ResultsContainsMatch)
{
    text = "Hello World";
    auto result = Search(" W");
    BOOST_REQUIRE_EQUAL(result.size(), 1);
    BOOST_CHECK_EQUAL(result.front().lineNumber, 1);
}

BOOST_AUTO_TEST_CASE(Results_SingleMatchInLine_ResultsContainsMatch)
{
    text = "<body>\nHello World\n</body>";
    auto result = Search(" W");
    BOOST_REQUIRE_EQUAL(result.size(), 1);
    BOOST_CHECK_EQUAL(result.front().lineNumber, 2);
}

BOOST_AUTO_TEST_CASE(Results_MatchesInDifferentLines_ResultsContainsMatches)
{
    text = "<body>\n<p>\nHello World\n</p>\nHello World!</body>";
    auto result = Search("World");
    BOOST_REQUIRE_EQUAL(result.size(), 2);
    BOOST_CHECK_EQUAL(result[0].lineNumber, 3);
    BOOST_CHECK_EQUAL(result[1].lineNumber, 5);
}

BOOST_AUTO_TEST_SUITE_END()
