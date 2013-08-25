#include <boost/test/unit_test.hpp>
#include "TextSearchAnalysis.hpp"
#include "HttpResponse.hpp"

struct TextSearchAnalysisTestFixture
{
    struct Observer : public TextSearchObserver {
        TextSearchResult result;
        bool notified = false;
        void NotifyMatch(const TextSearchResult &result){
            notified = true;
            this->result = result;
        }
    };

    TextSearchAnalysis analysis;
    Observer observer;
    std::string text;

    TextSearchAnalysisTestFixture()
    {
        analysis.SetObserver(observer);
    }

    void Search(std::string body, std::string searchFor)
    {
        HttpResponse response;
        response.uri = "http://www.test.com/";
        response.body = std::move(body);
        analysis.SearchFor(std::move(searchFor));
        analysis.Run(response);
    }
};

BOOST_FIXTURE_TEST_SUITE(TextSearchAnalysisTest, TextSearchAnalysisTestFixture)

BOOST_AUTO_TEST_CASE(EmptyText_NotNotified)
{
    Search("", "abc");
    BOOST_CHECK(!observer.notified);
}

BOOST_AUTO_TEST_CASE(HasMatch_NotNotified)
{
    Search("Hello World", "ll");
    BOOST_REQUIRE(observer.notified);
    BOOST_CHECK_EQUAL(observer.result.uri, "http://www.test.com/");
    BOOST_REQUIRE_EQUAL(observer.result.matches.size(), 1);
    BOOST_REQUIRE_EQUAL(observer.result.matches[0].lineNumber, 1);
}

BOOST_AUTO_TEST_SUITE_END()
