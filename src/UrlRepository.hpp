#ifndef URL_REPOSITORY_HPP
#define URL_REPOSITORY_HPP

#include <string>
#include <vector>


class UrlRepository
{
public:
    virtual void AddUrls(const std::vector<std::string>& newUrls) = 0;
    virtual bool HasUnvisitedUrls() const = 0;
    virtual std::string NextUnvisited() = 0;
    virtual void PopNextUnvisited() = 0;
    
    virtual ~UrlRepository() 
    {
    }
};

#endif  // URL_REPOSITORY_HPP
