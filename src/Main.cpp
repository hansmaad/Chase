#include <iostream>
#include <algorithm>
#include <iterator>
#include "Crawler.hpp"
#include "InMemoryUrlRepository.hpp"
#include "MultiHttpClient.hpp"
#include "CrawlerObserver.hpp"
#include "ExternalLinkFilter.hpp"
#include "TextSearchAnalysis.hpp"

struct ResponseReporter : CrawlerObserver
{
    void NotifyResponse(const HttpResponse& response) override
    {
        std::cout << response.status << ":  (" << response.ContentType() << ") "
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

struct TextSearchReporter : TextSearchObserver
{
    void NotifyMatch(const TextSearchResult& result) override
    {
        results.push_back(result);
    }

    void ReportAll(std::ostream& stream) const
    {
        for(auto& result : results)
        {
            stream << "Matches on " << result.uri << ":\n";
            for(auto& match : result.matches)
            {
                stream << "    " << match.lineNumber << "\n";
            }
        }
    }

    std::vector<TextSearchResult> results;
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Chase Url [Search]\n";
        return 0;
    }

    TextSearchReporter searchReporter;
    TextSearchAnalysis search;
    search.SetObserver(searchReporter);

    ResponseReporter reporter;
    InMemoryUrlRepository repository;
    MultiHttpClient httpClient;
    Crawler crawler{repository, httpClient};

    repository.AddUrls(std::vector<std::string>{argv[1]});

    crawler.SetLinkFilter(std::unique_ptr<LinkFilter>(new ExternalLinkFilter));
    crawler.AddObserver(reporter);
    if (argc > 2)
    {
        search.SearchFor(argv[2]);
        crawler.AddAnalysis(search);
    }
    crawler.Crawl();
    std::cout << "End\n";

    searchReporter.ReportAll(std::cout);
}
