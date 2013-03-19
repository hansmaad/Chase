#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include "BlockingQueue.hpp"
#include "HttpClient.hpp"
#include "UrlRepository.hpp"


class Crawler
{
public:
    Crawler(UrlRepository& repository, HttpClient& httpClient);

    void Crawl();

private:
    void FillUnvistedUrlQueue();

    BlockingQueue<std::string> unvisitedUrls;
    BlockingQueue<HttpResponse> httpResponseQueue;

    UrlRepository* urlRepository;
    HttpClient* httpClient;



};


#endif  // CRAWLER_HPP
