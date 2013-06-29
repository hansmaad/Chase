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
        std::cout << response.status << "  " << response.uri << "\n";
    }
};

int main(int argc, char* argv[])
{
    if (argc < 2)
        return 1;


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
