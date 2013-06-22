#include <future>
#include "Crawler.hpp"
#include "HtmlSearch.hpp"
#include <network/uri.hpp>

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

    for(;;)
    {
        ProcessNextResponse();
        if (ShouldStopProcess())
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

namespace
{

inline void ResolveLinks(HtmlSearchResult& searchResult,
                         const std::string& baseUrl)
{
    auto base = network::uri{baseUrl};
    for(auto& link : searchResult.links)
    {
        auto linkUrl = base.resolve(
                    network::uri{link},
                    network::uri_comparison_level::string_comparison);
        link = linkUrl.string();
    }
}

}

void Crawler::ProcessNextResponse()
{
    HtmlSearch htmlSearch;
    auto next = httpResponseQueue.Pop();
    auto searchResult = htmlSearch.Search(next.body);

    ResolveLinks(searchResult, next.uri);
    urlRepository->AddUrls(std::move(searchResult.links));
}

bool Crawler::ShouldStopProcess() const
{
    return !urlRepository->HasUnvisitedUrls();
}
