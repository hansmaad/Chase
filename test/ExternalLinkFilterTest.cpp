#include <boost/test/unit_test.hpp>
#include "ExternalLinkFilter.hpp"
#include <network/uri.hpp>

struct ExternalLinkFilterFixture
{
    network::uri base, link;
    ExternalLinkFilter filter;
};

BOOST_FIXTURE_TEST_SUITE(ExternalLinkFilterTest, ExternalLinkFilterFixture)

BOOST_AUTO_TEST_CASE(Internal_ReturnsTrue)
{
    base = network::uri{"http://a.com/b"};
    link = network::uri{"http://a.com/c"};
    BOOST_CHECK(true == filter.ShouldFollowLink(base, link));
}

BOOST_AUTO_TEST_CASE(External_ReturnsFalse)
{
    base = network::uri{"http://a.com/b"};
    link = network::uri{"http://b.com/c"};
    BOOST_CHECK(false == filter.ShouldFollowLink(base, link));
}


BOOST_AUTO_TEST_SUITE_END()


