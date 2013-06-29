#include <future>
#include <algorithm>
#include <network/uri.hpp>

#include "Crawler.hpp"
#include "HtmlSearch.hpp"
#include "CrawlerObserver.hpp"

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

bool Crawler::ShouldStopProcess() const
{
    return !urlRepository->HasUnvisitedUrls() && unvisitedUrls.Empty();
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


std::vector<std::string> Crawler::ResolveLinks(
        const std::string& pageUri,
        const std::vector<std::string>& links)
{
    auto baseUri = network::uri{pageUri};
    std::vector<std::string> urls;
    urls.reserve((links.size()));
    for(auto& link : links)
    {
        auto linkUrl = baseUri.resolve(
                    network::uri{link},
                    network::uri_comparison_level::string_comparison);
        if (!linkFilter || linkFilter->ShouldFollowLink(baseUri, linkUrl))
            urls.push_back(linkUrl.string());
    }

    return urls;
}

void Crawler::ProcessNextResponse()
{
    HtmlSearch htmlSearch;
    auto nextResponse = httpResponseQueue.Pop();
    NotifyObservers(nextResponse);
    auto searchResult = htmlSearch.Search(nextResponse.body);

    urlRepository->AddUrls(ResolveLinks(nextResponse.uri, searchResult.links));
}

void Crawler::NotifyObservers(const HttpResponse& response) const
{
    for(auto&& observer : observers)
    {
        observer->NotifyResponse(response);
    }
}

