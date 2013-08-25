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

    void Search(std::string searchText)
    {
        HttpResponse response;
        response.body = std::move(searchText);
        analysis.Run(response);
    }
};

BOOST_FIXTURE_TEST_SUITE(TextSearchAnalysisTest, TextSearchAnalysisTestFixture)

BOOST_AUTO_TEST_CASE(EmptyText_NotNotified)
{
    Search("abc");
    BOOST_CHECK(!observer.notified);
}


BOOST_AUTO_TEST_SUITE_END()
