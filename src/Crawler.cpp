#include <future>
#include <algorithm>

#include <network/uri.hpp>
#include "Crawler.hpp"
#include "HtmlSearch.hpp"
#include "CrawlerObserver.hpp"
#include "UriResolver.hpp"
#include "Status.hpp"



Crawler::Crawler(UrlRepository& repository, HttpClient& httpClient) :
    unvisitedUrls(10),
    httpResponseQueue(10),
    urlRepository(&repository),
    httpClient(&httpClient),
    inProgressCount(0)
{

}

void Crawler::Crawl()
{
    inProgressCount = 0;
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
    return inProgressCount == 0 &&              // http client not busy
           !urlRepository->HasUnvisitedUrls();  // no more uris
}

namespace
{

// Not in boost 1.53
bool operator==(const boost::string_ref& lh, const char* rh)
{
    boost::string_ref temp{rh};
    return lh == temp;
}


inline bool IsHttpScheme(const Uri& uri)
{
    auto scheme = uri.scheme();
    return scheme && (*scheme == "http" || *scheme == "https");
}
}

bool Crawler::ShouldFollowLink(const Uri& baseUri, const Uri& linkUri) const
{
    return IsHttpScheme(linkUri) &&
            (!linkFilter || linkFilter->ShouldFollowLink(baseUri, linkUri));
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
        ++inProgressCount;
    }
}

namespace
{

optional<network::uri> TryParseUri(const std::string& uri)
{
    try
    {
        return network::uri{uri};
    }
    catch(network::uri_syntax_error)
    {
        return optional<network::uri>{};
    }
}



}  // namespace


std::vector<std::string> Crawler::ResolveLinks(
        const std::string& pageUri,
        const std::vector<std::string>& links)
{
    auto baseUri = network::uri{pageUri};
    std::vector<std::string> urls;
    urls.reserve(links.size());
    for(auto& link : links)
    {
        auto resolvedUri = ResolveLink(baseUri, link);
        if (resolvedUri)
            urls.push_back(resolvedUri.get());
    }

    return urls;
}

optional<std::string> Crawler::ResolveLink(
        const Uri& baseUri, const std::string& link)
{
    auto linkUri = TryParseUri(link);
    if (!linkUri)
    {
        for(auto&& observer : observers)
        {
            observer->NotifyError(ErrorCode::InvalidUri, link);
        }
        return optional<std::string>{};
    }
    UriResolver uriResolver;
    auto resolvedUri = uriResolver.Resolve(baseUri, linkUri.get());
    if (ShouldFollowLink(baseUri, resolvedUri))
        return resolvedUri.string();
    return optional<std::string>{};
}

void Crawler::ProcessNextResponse()
{
    HtmlSearch htmlSearch;
    auto nextResponse = httpResponseQueue.Pop();
    --inProgressCount;

    NotifyObservers(nextResponse);

    if (Status::IsOk(nextResponse.status))
    {
        auto searchResult = htmlSearch.Search(nextResponse.body);
        urlRepository->AddUrls(ResolveLinks(nextResponse.uri, searchResult.links));
    }
    else if (Status::IsRedirect(nextResponse.status))
    {
        auto baseUri = network::uri{nextResponse.uri};
        auto resolved = ResolveLink(baseUri, nextResponse.Location());
        urlRepository->AddUrls(std::vector<std::string>{1, resolved.get()});
    }
}

void Crawler::NotifyObservers(const HttpResponse& response) const
{
    for(auto&& observer : observers)
    {
        observer->NotifyResponse(response);
    }
}



