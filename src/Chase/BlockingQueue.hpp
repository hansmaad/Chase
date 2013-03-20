#ifndef BLOCKING_QUEUE_HPP
#define BLOCKING_QUEUE_HPP

#include <utility>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <type_traits>


class BlockingQueueInterruptedException
{
};

/// BlockingQueue class template.
/// Producer threads, i.e. callers to Push methods will be blocked while the 
/// queue is full. Consumer threads, i.e. callers to Pop methods will be
/// blocked while the queue is empty.
/// @param Item The type of work items to store.
template<typename Element>
class BlockingQueue
{
public:
    /// Construction with max size of the queue.
    /// @param queueSize The maximum size of the queue.
    BlockingQueue(size_t queueSize) :
        maxSize(queueSize), interrupted(false), queue()
    {}

    /// Adds an element to the queue, waits while full.
    /// @param element A universal reference to the element to add.
    template<typename T>
    void Push(T&& element)
    {
        WaitAndPushElement(std::forward<T>(element));
        NotifyNotEmpty();
    }

    /// Tries to add an alement to the queue, never waits.
    /// @param item A const reference to an item that will 
    ///             be copied into the queue.
    /// @return false if queue is full.
    template<typename T>
    bool TryPush(T&& element)
    {
        Lock l = GetLock();
        if (IsFull()) 
            return false;
        queue.push(std::forward<T>(element));
        NotifyNotEmpty();
        return true;
    }

    /// Removes and returns the next element from the queue, waits while empty.
    /// @return The next item in the queue.
    Element Pop()
    {
        Element element(WaitAndPopElement());
        NotifyNotFull();
        return element;
    }

    /// Waits while queue is not empty. Returns when queue is empty.
    void WaitWhileNotEmpty()
    {
        Lock l = GetLock();        
        WaitWhileNotEmpty(l);
    }

    /// Returns true if the queue is empty.
    bool Empty() const
    {
        Lock l = GetLock();
        return IsEmpty();
    }

    /// After call the Push & Pop methods will throw a
    /// BlockingQueueInterruptedException. Use this method to notify waiting
    /// threads that no more elements will pushed to the queue.
    void Interrupt()
    {
        Lock lock = GetLock();
        interrupted = true;
        NotifyNotEmpty();
        NotifyNotFull();
    }

private:
    typedef std::queue<Element> Queue;
    typedef std::mutex Mutex;
    typedef std::condition_variable Condition;
    typedef std::unique_lock<Mutex> Lock;

    BlockingQueue(BlockingQueue&);
    void operator=(BlockingQueue&);

    bool IsFull() const
    {
        return queue.size() >= maxSize;
    }

    bool IsEmpty() const
    {
        return queue.empty();
    }

    Element WaitAndPopElement()
    {
        static_assert(
            std::is_nothrow_move_constructible<Element>::value ||
            std::is_nothrow_copy_constructible<Element>::value,
            "Element type mus be nothrow (move) constructible to "
            "guarantee strong exception safety.");

        Lock l = GetLock();
        WaitWhileEmpty(l);
        if (interrupted)
            throw BlockingQueueInterruptedException();
        Element i(std::move(queue.front()));  
        queue.pop();
        return i;
    }

    template<typename T>
    void WaitAndPushElement(T&& element)
    {
        Lock lock = GetLock();
        WaitWhileFull(lock);
        queue.push(std::forward<T>(element));
    }

    Lock GetLock()
    {
        return Lock(mutex);
    }

    void NotifyNotEmpty()
    {
        notEmpty.notify_one();
    }

    void NotifyNotFull()
    {
        notFull.notify_one();
    }

    void WaitWhileEmpty(Lock& lock)
    {
        while(IsEmpty() && !interrupted)
        {
            notEmpty.wait(lock);
        }
    }

    void WaitWhileFull(Lock& lock)
    {
        while(IsFull() && !interrupted)
            notFull.wait(lock);
    }

    void WaitWhileNotEmpty(Lock& lock)
    {
        while(!IsEmpty() && !interrupted)
            notFull.wait(lock);
    }
    
    size_t maxSize;
    bool interrupted;
    Queue queue;
    mutable Mutex mutex;
    Condition notEmpty, notFull;
};




#endif // BLOCKING_QUEUE_HPP

