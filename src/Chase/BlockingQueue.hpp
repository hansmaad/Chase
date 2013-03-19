/// \file blocking_queue.hpp
/// \brief Contains the BlockingQueue class template

#ifndef BLOCKING_QUEUE_HPP
#define BLOCKING_QUEUE_HPP

#include <utility>
#include <queue>

#define HAS_STD_THREAD

#ifdef HAS_STD_THREAD
#include <thread>
#include <mutex>
#include <condition_variable>
namespace Detail { class DefaultThreadPolicy; }
#else
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
namespace Detail { class DefaultThreadPolicy; }
#endif

/// BlockingQueue class template.
/// Producer threads, i.e. callers to Push methods will be blocked while the 
/// queue is full. Consumer threads, i.e. callers to Pull methods will be
/// blocked while the queue is empty.
/// @param Item The type of work items to store.
/// @param ThreadPolicy A policy for thread basics.
template<
    typename Item, 
    typename ThreadPolicy 
    = Detail::DefaultThreadPolicy
>
class BlockingQueue
{
public:
	/// Construction with max size of the queue.
    /// @param queueSize The maximum size of the queue.
	BlockingQueue(size_t queueSize) :
        maxSize_(queueSize), queue_()
    {}

    /// Adds an element to the queue, waits while full.
    /// @param element A universal reference to the element to add.
    template<typename T>
    void Push(T&& element)
    {
        PushElement(std::forward<T>(element));
        threading_.NotifyNotEmpty();
    }

    /// Tries to add an alement to the queue, never waits.
    /// @param item A const reference to an item that will 
    ///             be copied into the queue.
    /// @return false if queue is full.
    template<typename T>
    bool TryPush(T&& element)
    {
        Lock l = threading_.GetLock();
        if (Full()) return false;
        queue_.push(std::forward<T>(element));
        threading_.NotifyNotEmpty();
        return true;
    }

	/// Removes and returns the next element from the queue, waits while empty.
    /// @return The next item in the queue.
	Item Pull()
    {
        Item element(GetElement());
        threading_.NotifyNotFull();
	    return element;
    }

	/// Waits while queue is not empty.
	void WaitWhileNotEmpty()
    {
        Lock l = threading_.GetLock();
	    while(!IsEmpty())
            threading_.WaitWhileFull();
    }

    /// Returns true if the queue is empty.
    bool Empty() const
    {
        Lock l = threading_.GetLock();
	    return IsEmpty();
    }

private:
	typedef std::queue<Item> Queue;
	typedef typename ThreadPolicy::Lock Lock;

    BlockingQueue(BlockingQueue&);
    void operator=(BlockingQueue&);

    bool Full() const
    {
        return queue_.size() >= maxSize_;
    }

    bool IsEmpty() const
    {
        return queue_.empty();
    }

    Item GetElement()
    {
        Lock l = threading_.GetLock();
	    while(IsEmpty())
            threading_.WaitWhileEmpty(l);
        // TODO: Check move_if_noexcept here??
	    Item i(std::move(queue_.front()));  
	    queue_.pop();
	    return i;
    }

    template<typename T>
    void PushElement(T&& element)
    {
        Lock l = threading_.GetLock();
	    while(Full())
            threading_.WaitWhileFull(l);
        queue_.push(std::forward<T>(element));
    }

    size_t maxSize_;
	Queue queue_;
    mutable ThreadPolicy threading_;
};



namespace Detail
{

#ifdef HAS_STD_THREAD
/// Default threading policy.
class DefaultThreadPolicy
{
	typedef std::mutex Mutex;
	typedef std::condition_variable Condition;

public:
	typedef std::unique_lock<Mutex> Lock;

    /// Acquires a scoped lock.
    Lock GetLock()
    {
        return Lock(mutex_);
    }
    /// Notifies waiting consumer threads.
    void NotifyNotEmpty()
    {
        notEmpty_.notify_one();
    }
    /// Notifies waiting producer threads.
    void NotifyNotFull()
    {
        notFull_.notify_one();
    }
    /// Waits on empty condition.
    void WaitWhileEmpty(Lock& l)
    {
        notEmpty_.wait(l);
    }
    /// Waits on full condition.
    void WaitWhileFull(Lock& l)
    {
        notFull_.wait(l);
    }

private:
    Mutex mutex_;
	Condition notEmpty_, notFull_;
};


#else
/// Default threading policy.
class DefaultThreadPolicy
{
	typedef boost::mutex Mutex;
	typedef boost::condition_variable Condition;

public:
	typedef boost::unique_lock<Mutex> Lock;

    /// Acquires a scoped lock.
    Lock GetLock()
    {
        return Lock(mutex_);
    }
    /// Notifies waiting consumer threads.
    void NotifyNotEmpty()
    {
        notEmpty_.notify_one();
    }
    /// Notifies waiting producer threads.
    void NotifyNotFull()
    {
        notFull_.notify_one();
    }
    /// Waits on empty condition.
    void WaitWhileEmpty(Lock& l)
    {
        notEmpty_.wait(l);
    }
    /// Waits on full condition.
    void WaitWhileFull(Lock& l)
    {
        notFull_.wait(l);
    }

private:
    Mutex mutex_;
	Condition notEmpty_, notFull_;
};
#endif

}  // namespace detail

#endif // BLOCKING_QUEUE_HPP

