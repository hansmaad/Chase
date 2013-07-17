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
        std::cout << response.status << "  " << response.uri << "\n";
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



int main(int argc, char* argv[])
{

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
