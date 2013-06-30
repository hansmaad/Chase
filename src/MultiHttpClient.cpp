#include "MultiHttpClient.hpp"

#include <network/http/client.hpp>
#include <network/http/request.hpp>
#include <network/http/response.hpp>


namespace
{

    inline HttpResponse LoadCppNetlib(std::string nextUri)
    {
        using namespace network;
        try
        {
            http::client httpClient;
            auto request = http::client::request{nextUri};

            auto httpResponse = httpClient.get(request);

            HttpResponse response;
            response.uri = std::move(nextUri);
            response.body = body(httpResponse);
            response.status = status(httpResponse);

            return response;
        }
        catch(...)
        {
            HttpResponse response;
            response.uri = std::move(nextUri);
            response.status = 500;
            return response;
        }
    }

    inline HttpResponse Load(std::string nextUri)
    {
        return LoadCppNetlib(move(nextUri));
    }
}




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
    http::client httpClient;

    for(;;)
    {
        HttpResponse response;

        try
        {            
            auto nextUri = urlQueue->Pop();
            auto request = http::client::request{nextUri};

            auto httpResponse = httpClient.get(request);

            response.uri = std::move(nextUri);
            response.body = body(httpResponse);
            response.status = status(httpResponse);
        }
        catch(BlockingQueueInterruptedException)
        {
            break;
        }
        catch(...)
        {
            response.status = 0;
        }

        responseQueue->Push(std::move(response));
    }
}


