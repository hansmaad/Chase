#include <boost/test/unit_test.hpp>
#include "HttpClientStub.hpp"
#include "Crawler.hpp"
#include "InMemoryUrlRepository.hpp"


#include <future>


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
        return std::count(begin(httpClient.visits), end(httpClient.visits), url);
    }

    HttpClientStub httpClient;
    InMemoryUrlRepository repository;
    Crawler crawler;
};

BOOST_FIXTURE_TEST_SUITE(CrawlerTest, CrawlerTestFixture)

BOOST_AUTO_TEST_CASE(Crawl_SingleLink_LinkTargetAddedToRepository_BothVisitedOnce)
{
    httpClient.AddLink("http://www.google.de", "http://www.google.com");
    repository.AddUrls(std::vector<std::string>(1, "http://www.google.de"));
    crawler.Crawl();
    BOOST_CHECK(repository.Contains("http://www.google.com"));
    BOOST_CHECK_EQUAL(VisitedCount("http://www.google.de"), 1);
    BOOST_CHECK_EQUAL(VisitedCount("http://www.google.com"), 1);
}


BOOST_AUTO_TEST_CASE(Crawl_BackLink_BothVisitedOnlyOnce)
{
    httpClient.AddLink("http://www.google.de", "http://www.google.com");
    httpClient.AddLink("http://www.google.com", "http://www.google.de");
    repository.AddUrls(std::vector<std::string>(1, "http://www.google.de"));
    crawler.Crawl();
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
    repository.AddUrls(std::vector<std::string>(1, "http://a.de"));
    crawler.Crawl();
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
    repository.AddUrls(std::vector<std::string>(1, "http://a.de"));
    crawler.Crawl();
    BOOST_CHECK(repository.Contains("http://a.de"));
    BOOST_CHECK(repository.Contains("http://a.de/b"));
    BOOST_CHECK(repository.Contains("http://a.de/c"));
}

BOOST_AUTO_TEST_SUITE_END()
