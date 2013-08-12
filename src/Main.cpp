#include <iostream>
#include "Crawler.hpp"
#include "InMemoryUrlRepository.hpp"
#include "MultiHttpClient.hpp"
#include "CrawlerObserver.hpp"
#include "ExternalLinkFilter.hpp"

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

    std::cout << "End\n";
}
