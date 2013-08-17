#include <boost/test/unit_test.hpp>
#include "UriResolver.hpp"

typedef network::uri uri;

struct UriResolverTestFixture
{
    UriResolver resolver;
};

BOOST_FIXTURE_TEST_SUITE(UriResolverTest, UriResolverTestFixture)

BOOST_AUTO_TEST_CASE(Resolve_SameHost_ReturnsInput)
{
    uri base{"http://a.de/"}, link{"http://a.de/b"};
    uri resolved = resolver.Resolve(base, link);
    BOOST_CHECK_EQUAL(resolved.string(), "http://a.de/b");
}

BOOST_AUTO_TEST_CASE(Resolve_NoHost_ResolvedToAbsolute)
{
    uri base{"http://a.de/"}, link{"b"};
    uri resolved = resolver.Resolve(base, link);
    BOOST_CHECK_EQUAL(resolved.string(), "http://a.de/b");
}

BOOST_AUTO_TEST_CASE(Resolve_HasFragment_FragmentSrippedOff)
{
    uri base{"http://a.de/"}, link{"b#c"};
    uri resolved = resolver.Resolve(base, link);
    BOOST_CHECK_EQUAL(resolved.string(), "http://a.de/b");
}

BOOST_AUTO_TEST_SUITE_END()
