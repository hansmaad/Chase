#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include <memory>
#include <vector>
#include "BlockingQueue.hpp"
#include "HttpClient.hpp"
#include "UrlRepository.hpp"
#include "LinkFilter.hpp"


class CrawlerObserver;
struct HttpResponse;

class Crawler
{
public:
    Crawler(UrlRepository& repository, HttpClient& httpClient);

    void Crawl();

    void SetLinkFilter(std::unique_ptr<LinkFilter> filter)
    {
        linkFilter = move(filter);
    }

    void AddObserver(CrawlerObserver* observer)
    {
        observers.push_back(observer);
    }

private:
    void FillUnvistedUrlQueue();
    void ProcessNextResponse();
    bool ShouldStopProcess() const;
    std::vector<std::string> ResolveLinks(
            const std::string& pageUri,
            const std::vector<std::string>& searchResult);
    void NotifyObservers(const HttpResponse &response) const;

    BlockingQueue<std::string> unvisitedUrls;
    BlockingQueue<HttpResponse> httpResponseQueue;

    UrlRepository* urlRepository;
    HttpClient* httpClient;

    std::unique_ptr<LinkFilter> linkFilter;
    std::vector<CrawlerObserver*> observers;
};


#endif  // CRAWLER_HPP
