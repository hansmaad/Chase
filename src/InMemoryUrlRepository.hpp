#ifndef IN_MEMORY_URL_REPOSITORY_HPP
#define IN_MEMORY_URL_REPOSITORY_HPP

#include <unordered_set>
#include <deque>
#include "UrlRepository.hpp"


class InMemoryUrlRepository : public UrlRepository
{
public:
    typedef std::unordered_set<std::string>::const_iterator Iterator;

    void AddUrls(const std::vector<std::string>& newUrls) override;
    bool HasUnvisitedUrls() const override;
    std::string NextUnvisited() override;
    void PopNextUnvisited() override;

    bool Contains(const std::string& url)
    {
        return urls.find(url) != end(urls);
    }


//private:
    std::unordered_set<std::string> urls;
    std::deque<std::string> unvisitedUrls;
};


inline InMemoryUrlRepository::Iterator begin(const InMemoryUrlRepository& r)
{
    return begin(r.urls);
}

inline InMemoryUrlRepository::Iterator end(const InMemoryUrlRepository& r)
{
    return end(r.urls);
}

#endif  // IN_MEMORY_URL_REPOSITORY_HPP
