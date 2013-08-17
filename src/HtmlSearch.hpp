#ifndef HTML_SEARCH_HPP
#define HTML_SEARCH_HPP


#include <string>
#include <memory>
#include <vector>
#include "AttachedSearch.hpp"

struct HtmlSearchResult
{
	std::vector<std::string> links;
};


class HtmlSearch
{
public:
    HtmlSearch();
    ~HtmlSearch();
    HtmlSearchResult Search(const std::string& htmlContent);
    void AttachSearch(AttachedSearch* search);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

#endif  // HTML_SEARCH_HPP
