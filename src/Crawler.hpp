#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include <memory>
#include <vector>
#include <boost/optional.hpp>
#include "BlockingQueue.hpp"
#include "HttpClient.hpp"
#include "UrlRepository.hpp"
#include "LinkFilter.hpp"
#include "Analysis.hpp"

template <typename T>
using optional = boost::optional<T>;

class CrawlerObserver;
struct HttpResponse;
namespace network { class uri; }
typedef network::uri Uri;

class Crawler
{
public:
    Crawler(UrlRepository& repository, HttpClient& httpClient);

    void Crawl();

    void SetLinkFilter(std::unique_ptr<LinkFilter> filter)
    {
        linkFilter = move(filter);
    }

    void AddObserver(CrawlerObserver& observer)
    {
        observers.push_back(&observer);
    }

    void AddAnalysis(Analysis& analysis)
    {
        analyses.push_back(&analysis);
    }

private:
    void FillUnvistedUrlQueue();
    void ProcessNextResponse();
    bool ShouldStopProcess() const;
    bool ShouldFollowLink(const Uri& baseUri, const Uri& linkUri) const;
    std::vector<std::string> ResolveLinks(
            const std::string& pageUri,
            const std::vector<std::string>& searchResult);
    optional<std::string> ResolveLink(
            const Uri& baseUri, const std::string& link);

    void NotifyObservers(const HttpResponse &response) const;
    void RunAnalyses(const HttpResponse &response) const;

    BlockingQueue<std::string> unvisitedUrls;
    BlockingQueue<HttpResponse> httpResponseQueue;

    UrlRepository* urlRepository;
    HttpClient* httpClient;

    std::unique_ptr<LinkFilter> linkFilter;
    std::vector<CrawlerObserver*> observers;
    std::vector<Analysis*> analyses;
    unsigned inProgressCount;
};


#endif  // CRAWLER_HPP
