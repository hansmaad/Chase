#include <future>
#include "Crawler.hpp"
#include "HtmlSearch.hpp"


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

    HtmlSearch htmlSearch;
    for(;;)
    {
        auto next = httpResponseQueue.Pop();
        auto searchResult = htmlSearch.Search(next.body);
        urlRepository->AddUrls(std::move(searchResult.links));
        if (!urlRepository->HasUnvisitedUrls())
        {
            unvisitedUrls.Interrupt();
            break;
        }
        FillUnvistedUrlQueue();
    }

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

        urlRepository->PopNextUnvisited();
    }
}