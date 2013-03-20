#include <boost/test/unit_test.hpp>
#include "InMemoryUrlRepository.hpp"

struct InMemoryUrlRepositoryTestFixture
{
    template<typename... T>
    void AddUrls(T&&... values)
    {
        std::vector<std::string> urls;
        Add(urls, values...);
        repository.AddUrls(std::move(urls));
    }

    InMemoryUrlRepository repository;

private:
    template<typename T>
    void Add(std::vector<std::string>& v, T&& value)
    {
        v.push_back(std::forward<T>(value));
    }
 
    template<typename U, typename... T>
    void Add(std::vector<std::string>& v, U&& value, T&&... values)
    {
        v.push_back(std::forward<U>(value));
        Add(v, values...);
    }
};


BOOST_FIXTURE_TEST_SUITE(InMemoryUrlRepositoryTest, InMemoryUrlRepositoryTestFixture)

BOOST_AUTO_TEST_CASE(HasUnvisitedUrls_IsEmpty_IsFalse)
{	
    BOOST_CHECK_EQUAL(repository.HasUnvisitedUrls(), false);
}

BOOST_AUTO_TEST_CASE(HasUnvisitedUrls_AddedUrl_IsTrue)
{	
    AddUrls("http://www.google.de");
    BOOST_CHECK_EQUAL(repository.HasUnvisitedUrls(), true);
}

BOOST_AUTO_TEST_CASE(HasUnvisitedUrls_AddedUrlWasPopped_IsFalse)
{	
    AddUrls("http://www.google.de");
    auto next = repository.NextUnvisited();
    repository.PopNextUnvisited();
    BOOST_CHECK_EQUAL(next, "http://www.google.de");
    BOOST_CHECK_EQUAL(repository.HasUnvisitedUrls(), false);
}

BOOST_AUTO_TEST_CASE(HasUnvisitedUrls_AddedUrlWasPoppedAndAddedAgain_IsFalse)
{	
    AddUrls("http://www.google.de");
    repository.PopNextUnvisited();
    AddUrls("http://www.google.de");
    BOOST_CHECK_EQUAL(repository.HasUnvisitedUrls(), false);
}

BOOST_AUTO_TEST_CASE(HasUnvisitedUrls_AddedUrlWasPoppedAndNewUrlAdded_IsTrue)
{	
    AddUrls("http://www.google.de");
    repository.PopNextUnvisited();
    AddUrls("http://www.google.de", "http://www.google.com");
    BOOST_CHECK_EQUAL(repository.HasUnvisitedUrls(), true);
    auto next = repository.NextUnvisited();
    repository.PopNextUnvisited();
    BOOST_CHECK_EQUAL(next, "http://www.google.com");
    BOOST_CHECK_EQUAL(repository.HasUnvisitedUrls(), false);
}

BOOST_AUTO_TEST_SUITE_END()
