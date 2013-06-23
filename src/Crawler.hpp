#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include <memory>
#include <vector>
#include "BlockingQueue.hpp"
#include "HttpClient.hpp"
#include "UrlRepository.hpp"
#include "LinkFilter.hpp"

class Crawler
{
public:
    Crawler(UrlRepository& repository, HttpClient& httpClient);

    void Crawl();

    void SetLinkFilter(std::unique_ptr<LinkFilter> filter);

private:
    void FillUnvistedUrlQueue();
    void ProcessNextResponse();
    bool ShouldStopProcess() const;

    BlockingQueue<std::string> unvisitedUrls;
    BlockingQueue<HttpResponse> httpResponseQueue;

    UrlRepository* urlRepository;
    HttpClient* httpClient;

    std::unique_ptr<LinkFilter> linkFilter;
};


#endif  // CRAWLER_HPP
