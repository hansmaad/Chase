#include <boost/test/unit_test.hpp>
#include <string>
#include <thread>
#include <chrono>
#include "BlockingQueue.hpp"

BOOST_AUTO_TEST_SUITE(BlockingQueueTest)

BOOST_AUTO_TEST_CASE(Pop_HasElement_ReturnsElement)
{
    BlockingQueue<int> q(10);
    q.Push(1);
    int result = q.Pop();
    BOOST_CHECK_EQUAL(result, 1);
}


BOOST_AUTO_TEST_CASE(Pop_WaitsOnEmptyQueue_ThrowsInterruptedExceptionWhenInterrupted)
{
    BlockingQueue<std::string> queue(5);
    std::thread t([&queue]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        queue.Interrupt();
    });

    bool interrupted = false;
    try
    {
        queue.Pop(); // Should wait until thread t interrupts the queue.
    }
    catch(BlockingQueueInterruptedException)
    {
        interrupted = true;
    }
    BOOST_CHECK(interrupted);
    t.join();
}

BOOST_AUTO_TEST_SUITE_END()
