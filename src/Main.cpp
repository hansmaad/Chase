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
};


void TestCppNetlib(const char* url)
{
    using namespace network;

    http::client httpClient;
    auto request = http::client::request{url};
    auto httpResponse = httpClient.get(request);
    std::cout << body(httpResponse).size() << "\n";
}

void TestCurl(const char* url)
{
    struct Content
    {
        std::vector<char> data;
        static size_t Write(char * data, size_t size, size_t nmemb, void * p)
        {
            return static_cast<Content*>(p)->WriteImpl(data, size, nmemb);
        }

        size_t WriteImpl(char* ptr, size_t size, size_t nmemb)
        {
            data.insert(end(data), ptr, ptr + size * nmemb);
            return size * nmemb;
        }
    };

    Content content;

    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Content::Write);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    content.data.push_back('\0');

    std::cout << content.data.size() << "\n";
}



int main(int argc, char* argv[])
{

    if (argc < 2)
    {
        TestCppNetlib("http://www.poddox.com/");
        //TestCurl("http://www.poddox.com/");
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
