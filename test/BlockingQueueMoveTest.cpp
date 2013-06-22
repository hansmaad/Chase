#include <boost/test/unit_test.hpp>

#include <string>
#include "BlockingQueue.hpp"

BOOST_AUTO_TEST_SUITE(BlockingQueueMoveTest)

class Uncopyable
{
public:
    Uncopyable(const char* t) : text(t){}
    Uncopyable(Uncopyable&& other) :
        text(move(other.text))
    {
        
    }

    Uncopyable& operator=(Uncopyable&& other)
    {
        text = move(other.text);
        return *this;
    }
    std::string text;
private:
    Uncopyable(Uncopyable&);
    void operator=(Uncopyable&);
};

BOOST_AUTO_TEST_CASE(Pop_HasElement_ReturnsElement)
{
    BlockingQueue<Uncopyable> q(3);
    q.Push(Uncopyable("hallo"));
    Uncopyable u = q.Pop();
    BOOST_CHECK_EQUAL(u.text, "hallo");
}

BOOST_AUTO_TEST_SUITE_END()
