#include <boost/test/unit_test.hpp>
#include "InMemoryUrlRepository.hpp"

struct InMemoryUrlRepositoryTestFixture
{
	InMemoryUrlRepositoryTestFixture()
	{

	}

    void AddUrl(std::string url)
    {
        std::vector<std::string> urls;
        urls.push_back(std::move(url));
        repository.AddUrls(std::move(urls));
    }

    InMemoryUrlRepository repository;
};


BOOST_FIXTURE_TEST_SUITE(InMemoryUrlRepositoryTest, InMemoryUrlRepositoryTestFixture)

BOOST_AUTO_TEST_CASE(HasUnvisitedUrls_IsEmpty_IsFalse)
{	
    BOOST_CHECK_EQUAL(repository.HasUnvisitedUrls(), false);
}

BOOST_AUTO_TEST_CASE(HasUnvisitedUrls_AddedUrl_IsTrue)
{	
    AddUrl("http://www.google.de");
    BOOST_CHECK_EQUAL(repository.HasUnvisitedUrls(), true);
}

BOOST_AUTO_TEST_CASE(HasUnvisitedUrls_AddedUrlWasPopped_IsFalse)
{	
    AddUrl("http://www.google.de");
    auto next = repository.NextUnvisited();
    repository.PopNextUnvisited();
    BOOST_CHECK_EQUAL(next, "http://www.google.de");
    BOOST_CHECK_EQUAL(repository.HasUnvisitedUrls(), false);
}

BOOST_AUTO_TEST_CASE(HasUnvisitedUrls_AddedUrlWasPoppedAndAddedAgain_IsFalse)
{	
    AddUrl("http://www.google.de");
    repository.PopNextUnvisited();
    AddUrl("http://www.google.de");
    BOOST_CHECK_EQUAL(repository.HasUnvisitedUrls(), false);
}


BOOST_AUTO_TEST_SUITE_END()