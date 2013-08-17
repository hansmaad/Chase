#include <boost/test/unit_test.hpp>
#include "Status.hpp"

BOOST_AUTO_TEST_SUITE(StatusTest)

BOOST_AUTO_TEST_CASE(IsOk_Is200_True)
{
    BOOST_CHECK(Status::IsOk(200));
}

BOOST_AUTO_TEST_CASE(IsOk_Is400_False)
{
    BOOST_CHECK(!Status::IsOk(400));
}

BOOST_AUTO_TEST_CASE(IsRedirect_True)
{
    BOOST_CHECK(Status::IsRedirect(300));
    BOOST_CHECK(Status::IsRedirect(301));
    BOOST_CHECK(Status::IsRedirect(302));
    BOOST_CHECK(Status::IsRedirect(303));
    BOOST_CHECK(Status::IsRedirect(307));
}

BOOST_AUTO_TEST_CASE(IsRedirect_False)
{
    BOOST_CHECK(!Status::IsRedirect(304));
    BOOST_CHECK(!Status::IsRedirect(305));
    BOOST_CHECK(!Status::IsRedirect(200));
    BOOST_CHECK(!Status::IsRedirect(500));
}

BOOST_AUTO_TEST_SUITE_END()
