#ifndef MULTIHTTPCLIENT_HPP
#define MULTIHTTPCLIENT_HPP

#include "HttpClient.hpp"
#include <future>
#include <vector>

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

    std::vector<std::future<void>> threads;
    InputQueue* urlQueue;
    ResponseQueue* responseQueue;
};

#endif // MULTIHTTPCLIENT_HPP
