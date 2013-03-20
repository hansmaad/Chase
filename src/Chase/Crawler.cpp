#include "Crawler.hpp"
#include <future>


Crawler::Crawler(UrlRepository& repository, HttpClient& httpClient) :
    unvisitedUrls(10),
    httpResponseQueue(10),
    urlRepository(&repository),
    httpClient(&httpClient)
{

}

void Crawler::Crawl()
{
    FillUnvistedUrlQueue();
    httpClient->StartAsync(unvisitedUrls, httpResponseQueue);
    httpClient->Wait();
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