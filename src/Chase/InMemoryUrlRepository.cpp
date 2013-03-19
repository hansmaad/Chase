#include "InMemoryUrlRepository.hpp"



void InMemoryUrlRepository::AddUrls(std::vector<std::string> newUrls)
{
    for(auto& url : newUrls)
    {
        auto inserted = urls.insert(std::move(url));
        if (inserted.second)
        {
            unvisitedUrls.push_back(*inserted.first);
        }       
    }
}

bool InMemoryUrlRepository::HasUnvisitedUrls() const
{
    return !unvisitedUrls.empty();
}
    
std::string InMemoryUrlRepository::NextUnvisited()
{
    return unvisitedUrls.front();
}

void InMemoryUrlRepository::PopNextUnvisited()
{
    unvisitedUrls.pop_front();
}

