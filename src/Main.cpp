#include <iostream>
#include "Crawler.hpp"
#include "InMemoryUrlRepository.hpp"
#include "MultiHttpClient.hpp"

int main()
{
    InMemoryUrlRepository repository;
    MultiHttpClient httpClient;
    Crawler crawler{repository, httpClient};

    repository.AddUrls(std::vector<std::string>{"http://www.test.com"});
    crawler.Crawl();

    for(auto i = begin(repository.urls), e = end(repository.urls); i != e; ++i)
        std::cout << *i << "\n";
}
