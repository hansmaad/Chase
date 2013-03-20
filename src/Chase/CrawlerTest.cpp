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

    HttpClientStub httpClient;
    InMemoryUrlRepository repository;
    Crawler crawler;
};

BOOST_FIXTURE_TEST_SUITE(CrawlerTest, CrawlerTestFixture)

BOOST_AUTO_TEST_CASE(HasUnvisitedUrls_IsEmpty_IsFalse)
{
    //crawler.Crawl();
    auto f = std::async(std::launch::async, []() { });
    f.wait();
}



BOOST_AUTO_TEST_SUITE_END()
