#include <boost/test/unit_test.hpp>
#include "HttpResponse.hpp"

struct HttpResponseTestFixture
{
    std::string empty;
    HttpResponse response;
    void AddHeader(std::string key, std::string value)
    {
        response.header.insert(std::make_pair(key, value));
    }
};

BOOST_FIXTURE_TEST_SUITE(HttpResponseTest, HttpResponseTestFixture)

BOOST_AUTO_TEST_CASE(ContentType_NotSet_IsUnknown)
{
    BOOST_CHECK_EQUAL(response.ContentType(), "unknown");
}

BOOST_AUTO_TEST_CASE(Location_NotSet_Empty)
{
    BOOST_CHECK_EQUAL(response.Location(), empty);
}

BOOST_AUTO_TEST_CASE(ContentType_ReturnsContentType)
{
    AddHeader("Content-Type", "text/html");
    BOOST_CHECK_EQUAL(response.ContentType(), "text/html");
}

BOOST_AUTO_TEST_CASE(Location_ReturnsLocation)
{
    AddHeader("Location", "http://www.gurgel.de/");
    BOOST_CHECK_EQUAL(response.Location(), "http://www.gurgel.de/");
}


BOOST_AUTO_TEST_SUITE_END()

