#include <boost/test/unit_test.hpp>
#include <future>

#include "HttpClientStub.hpp"
#include "Crawler.hpp"
#include "InMemoryUrlRepository.hpp"
#include "ExternalLinkFilter.hpp"
#include "CrawlerObserver.hpp"
#include "Analysis.hpp"


struct CrawlerTestFixture
{
    CrawlerTestFixture() :
        httpClient(),
        repository(),
        crawler(repository, httpClient)
    {

    }

    unsigned VisitedCount(const std::string& url)
    {
        return std::count(begin(repository), end(repository), url);
    }

    void StartCrawling(std::string startUrl)
    {
        repository.AddUrls(std::vector<std::string>(1, move(startUrl)));
        crawler.Crawl();
    }

    HttpClientStub httpClient;
    InMemoryUrlRepository repository;
    Crawler crawler;
};

BOOST_FIXTURE_TEST_SUITE(CrawlerTest, CrawlerTestFixture)

BOOST_AUTO_TEST_CASE(Crawl_SingleLink_LinkTargetAddedToRepository_BothVisitedOnce)
{
    httpClient.AddLink("http://www.google.de", "http://www.google.com");
    StartCrawling("http://www.google.de");
    BOOST_CHECK(repository.Contains("http://www.google.com"));
    BOOST_CHECK_EQUAL(VisitedCount("http://www.google.de"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://www.google.com"), 1);
}


BOOST_AUTO_TEST_CASE(Crawl_BackLink_BothVisitedOnlyOnce)
{
    httpClient.AddLink("http://www.google.de", "http://www.google.com");
    httpClient.AddLink("http://www.google.com", "http://www.google.de");
    StartCrawling("http://www.google.de");
    BOOST_CHECK(repository.Contains("http://www.google.com"));
    BOOST_CHECK_EQUAL(VisitedCount("http://www.google.de"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://www.google.com"), 1);
}

BOOST_AUTO_TEST_CASE(Crawl_LinkChain_AllAddedToRepository)
{
    httpClient.AddLink("http://a.de", "http://b.de");
    httpClient.AddLink("http://b.de", "http://c.de");
    httpClient.AddLink("http://c.de", "http://d.de");
    httpClient.AddLink("http://d.de", "http://e.de");
    httpClient.AddLink("http://e.de", "http://c.de");
    StartCrawling("http://a.de");
    BOOST_CHECK(repository.Contains("http://a.de"));
    BOOST_CHECK(repository.Contains("http://b.de"));
    BOOST_CHECK(repository.Contains("http://c.de"));
    BOOST_CHECK(repository.Contains("http://d.de"));
    BOOST_CHECK(repository.Contains("http://e.de"));
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://b.de"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://c.de"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://d.de"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://e.de"), 1);
}

BOOST_AUTO_TEST_CASE(Crawl_HasRelativeLinks_LinksResolved)
{
    httpClient.AddLink("http://a.de", "b");
    httpClient.AddLink("http://a.de", "c");
    StartCrawling("http://a.de");
    BOOST_CHECK(repository.Contains("http://a.de"));
    BOOST_CHECK(repository.Contains("http://a.de/b"));
    BOOST_CHECK(repository.Contains("http://a.de/c"));
}

BOOST_AUTO_TEST_CASE(Crawl_WithExternalFilter_DoesNotFollowExternalLinks)
{
    crawler.SetLinkFilter(std::unique_ptr<LinkFilter>(new ExternalLinkFilter));
    httpClient.AddLink("http://a.de", "b");
    httpClient.AddLink("http://a.de/b", "http://b.de");
    httpClient.AddLink("http://a.de/b", "http://a.de/c");
    StartCrawling("http://a.de");
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/b"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/c"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://b.de"), 0);
}

BOOST_AUTO_TEST_CASE(Crawl_HasObserver_AllObserverNotified)
{
    int counter = 0;
    struct ObserverMock : CrawlerObserver
    {
        int* c;
        void NotifyResponse(const HttpResponse&) override {
            ++(*c);
        }
    };
    ObserverMock m1, m2;
    m1.c = m2.c = &counter;

    crawler.AddObserver(m1);
    crawler.AddObserver(m2);
    httpClient.AddLink("http://www.google.de", "http://www.google.com");
    StartCrawling("http://www.google.de");
    BOOST_CHECK_EQUAL(counter, 4);
}

BOOST_AUTO_TEST_CASE(Crawl_HasToWaitForHttpClient)
{
    httpClient.AddLink("http://a.de", "b");
    httpClient.AddLink("http://a.de", "c");
    httpClient.AddLink("http://a.de/b", "c");
    httpClient.AddLink("http://a.de/c", "x");

    httpClient.sleepMilliseconds = 100;
    StartCrawling("http://a.de");

    BOOST_CHECK_EQUAL(VisitedCount("http://a.de"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/b"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/c"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/x"), 1);

}

BOOST_AUTO_TEST_CASE(Crawl_InvalidUri_NotifiesObserver)
{
    struct ObserverMock : CrawlerObserver
    {
        std::string msg;
        ErrorCode errorCode;
        void NotifyError(ErrorCode error, std::string content) override {
            msg = content;
            errorCode = error;
        }
    };
    ObserverMock mock;
    crawler.AddObserver(mock);
    httpClient.AddLink("http://a.com/", "invalid\nwithnewline");
    StartCrawling("http://a.com/");
    BOOST_CHECK_EQUAL(mock.msg, "invalid\nwithnewline");
    BOOST_CHECK(mock.errorCode == ErrorCode::InvalidUri);
}

BOOST_AUTO_TEST_CASE(Crawl_VisitsHttpAndHttpsOnly)
{
    httpClient.AddLink("http://a.de", "http://a.de/b");
    httpClient.AddLink("http://a.de", "https://a.de/c");
    httpClient.AddLink("http://a.de", "mailto:hans@a.de");
    httpClient.AddLink("http://a.de", "ftp://a.de/d");
    StartCrawling("http://a.de");
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/b"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("https://a.de/c"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("mailto:hans@a.de"), 0);
    BOOST_CHECK_EQUAL(VisitedCount("ftp://a.de/d"), 0);
}

BOOST_AUTO_TEST_CASE(Crawl_DoesntVisitFragmentOnKnownPage)
{
    httpClient.AddLink("http://a.de/a", "http://a.de/b");
    httpClient.AddLink("http://a.de/b", "http://a.de/a#c");

    StartCrawling("http://a.de/a");
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/a"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/b"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/a#c"), 0);
}

BOOST_AUTO_TEST_CASE(Crawl_DoesVisitFragmentOnUnknownPage)
{
    httpClient.AddLink("http://a.de/a", "http://a.de/b#c");

    StartCrawling("http://a.de/a");
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/a"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/b"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/b#c"), 0);
}

BOOST_AUTO_TEST_CASE(Crawl_VisitsRedirectTarget)
{
    httpClient.AddRedirect("http://a.de/a", "http://a.de/b");

    StartCrawling("http://a.de/a");
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/a"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/b"), 1);
}

BOOST_AUTO_TEST_CASE(Crawl_VisitsRelativeRedirectTarget)
{
    httpClient.AddRedirect("http://a.de/a", "b");

    StartCrawling("http://a.de/a");
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/a"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://a.de/b"), 1);
}

BOOST_AUTO_TEST_CASE(RunsAnalysesOnResponse)
{
    struct AnalysisMock : Analysis {
        std::string uri;
        void Run(const HttpResponse& response) override {
            uri = response.uri;
        }
    } analysis1, analysis2;
    crawler.AddAnalysis(analysis1);
    crawler.AddAnalysis(analysis2);
    StartCrawling("http://a.de/");
    BOOST_CHECK_EQUAL(analysis1.uri, "http://a.de/");
    BOOST_CHECK_EQUAL(analysis2.uri, "http://a.de/");
}

BOOST_AUTO_TEST_SUITE_END()
