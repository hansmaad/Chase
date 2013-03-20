#ifndef HTTP_CLIENT_STUB_HPP
#define HTTP_CLIENT_STUB_HPP

#include <thread>
#include "HttpClient.hpp"

class HttpClientStub : public HttpClient
{
public:
    void StartAsync(InputQueue& urlQueue, ResponseQueue& responseQueue) override
    {
        this->urlQueue = &urlQueue;
        this->responseQueue = &responseQueue;
        
        thread = std::thread([]() { });
    }

    void Wait() override
    {
        if (thread.joinable())
            thread.join();
    }

    ~HttpClientStub()
    {

    }

private:
    void Run()
    {

    }

    std::thread thread;
    InputQueue* urlQueue;
    ResponseQueue* responseQueue;
};

#endif  // HTTP_CLIENT_STUB_HPP
