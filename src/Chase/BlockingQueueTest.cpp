#include <boost/test/unit_test.hpp>

#include "BlockingQueue.hpp"

BOOST_AUTO_TEST_SUITE(BlockingQueueTest)

BOOST_AUTO_TEST_CASE(Pull_HasElement_ReturnsElement)
{
    BlockingQueue<int> q(10);
    q.Push(1);
    int result = q.Pull();
    BOOST_CHECK_EQUAL(result, 1);
}

BOOST_AUTO_TEST_SUITE_END()
