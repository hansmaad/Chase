#ifndef IN_MEMORY_URL_REPOSITORY_HPP
#define IN_MEMORY_URL_REPOSITORY_HPP

#include <unordered_set>
#include <deque>
#include "UrlRepository.hpp"


class InMemoryUrlRepository : public UrlRepository
{
public:
    void AddUrls(std::vector<std::string> newUrls) override;
    bool HasUnvisitedUrls() const override;
    std::string NextUnvisited() override;
    void PopNextUnvisited() override;

private:
    std::unordered_set<std::string> urls;
    std::deque<std::string> unvisitedUrls;
};

#endif  // IN_MEMORY_URL_REPOSITORY_HPP