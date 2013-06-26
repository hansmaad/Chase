#include "MultiHttpClient.hpp"

#include <network/http/client.hpp>
#include <network/http/request.hpp>
#include <network/http/response.hpp>

MultiHttpClient::MultiHttpClient()
{
}

void MultiHttpClient::StartAsync(
        HttpClient::InputQueue &urlQueue,
        HttpClient::ResponseQueue &responseQueue)
{
    this->urlQueue = &urlQueue;
    this->responseQueue = &responseQueue;
    thread = std::async(std::launch::async, [this]() { Run(); });
}

void MultiHttpClient::Wait()
{
    thread.wait();
}

void MultiHttpClient::Run()
{
    using namespace network;

    for(;;)
    {
        try
        {
            http::client httpClient;
            auto uri = urlQueue->Pop();
            auto request = http::client::request{uri};

            auto httpResponse = httpClient.get(request);

            HttpResponse response;
            response.uri = uri;
            response.body = body(httpResponse);

            responseQueue->Push(std::move(response));
        }
        catch(BlockingQueueInterruptedException)
        {
            break;
        }
    }
}
