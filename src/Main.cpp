#include <iostream>
#include "Crawler.hpp"
#include "InMemoryUrlRepository.hpp"
#include "MultiHttpClient.hpp"
#include "CrawlerObserver.hpp"
#include "ExternalLinkFilter.hpp"



#include "curl/curl.h"
#include <network/http/client.hpp>
#include <network/http/request.hpp>
#include <network/http/response.hpp>




struct ResponseReporter : CrawlerObserver
{
    void NotifyResponse(const HttpResponse& response) override
    {
        std::cout << response.status << ":  (" << response.contentType << ") "
                  << response.uri << " length: " << response.body.length() << "\n";
    }

    void NotifyError(ErrorCode error, std::string content) override
    {
        switch (error)
        {
        case ErrorCode::InvalidUri:
            std::cout << "Invalid URI: " << content << "\n";
            break;
        }
    }
};

void test()
{
    using namespace std;
    using namespace network;
    HttpResponse response;

    http::client httpClient;
    auto request = http::client::request{"http://www.poddox.com/de/ipod-software-download/poddox_xi.zip"};
    request << header("Connection", "close");

    auto httpHeadResponse = httpClient.head(request);
    typedef decltype(headers(httpHeadResponse))::container_type headers_map;

    headers_map header = headers(httpHeadResponse);
    auto it = header.find("Content-Type");
    if (it != end(header) && it->second == "text/html")
    {
        std::cout << "html content\n";
    }
    else
    {
        std::cout << "non html content\n";
    }

    for(const auto& i : header)
    {
        std::cout << i.first << " " << i.second << "\n";
    }

    //auto httpResponse = httpClient.get(request);

}

int main(int argc, char* argv[])
{


    //test();
    //return 0;


    if (argc < 2)
    {
        std::cout << "Usage: Chase Url\n";
        return 0;
    }


    ResponseReporter reporter;
    InMemoryUrlRepository repository;
    MultiHttpClient httpClient;
    Crawler crawler{repository, httpClient};

    repository.AddUrls(std::vector<std::string>{argv[1]});

    crawler.SetLinkFilter(std::unique_ptr<LinkFilter>(new ExternalLinkFilter));
    crawler.AddObserver(&reporter);
    crawler.Crawl();

    std::vector<std::string> urls{begin(repository.urls), end(repository.urls)};
    std::cout << "End\n";

}
