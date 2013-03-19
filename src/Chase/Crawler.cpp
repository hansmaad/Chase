#include "Crawler.hpp"
#include <future>

void Crawler::Crawl()
{
    FillUnvistedUrlQueue();
    auto httpFuture = std::async(
        std::launch::async,
        [this]
    {
        httpClient->Start(unvisitedUrls, httpResponseQueue);
    });

    
}

void Crawler::FillUnvistedUrlQueue()
{
    while(urlRepository->HasUnvisitedUrls())
    {
        bool queueIsFull = !unvisitedUrls.TryPush(
            urlRepository->NextUnvisited());
        if (queueIsFull)
            break;
    }
}