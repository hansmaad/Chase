#include "MultiHttpClient.hpp"

#include "SingleHttpClient.hpp"


MultiHttpClient::MultiHttpClient()
{
}

void MultiHttpClient::StartAsync(
        HttpClient::InputQueue &urlQueue,
        HttpClient::ResponseQueue &responseQueue)
{
    this->urlQueue = &urlQueue;
    this->responseQueue = &responseQueue;
    for(int i = 0; i < 5; ++i)
        threads.push_back(std::async(std::launch::async, [this]() { Run(); }));
}

void MultiHttpClient::Wait()
{
    for(auto&& thread : threads)
        thread.wait();
}

void MultiHttpClient::Run()
{
    SingleHttpClient httpClient;

    for(;;)
    {
        HttpResponse response;

        try
        {            
            response = httpClient.Get(urlQueue->Pop());

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
