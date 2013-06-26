#ifndef MULTIHTTPCLIENT_HPP
#define MULTIHTTPCLIENT_HPP

#include "HttpClient.hpp"
#include <future>

class MultiHttpClient : public HttpClient
{
public:
    MultiHttpClient();

    void StartAsync(
        InputQueue& urlQueue,
        ResponseQueue& responseQueue) override;

    void Wait() override;

private:
    void Run();

    std::future<void> thread;
    InputQueue* urlQueue;
    ResponseQueue* responseQueue;
};

#endif // MULTIHTTPCLIENT_HPP
