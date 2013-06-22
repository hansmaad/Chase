#ifndef HTML_SEARCH_HPP
#define HTML_SEARCH_HPP

#include <vector>
#include <string>

struct HtmlSearchResult
{
	std::vector<std::string> links;
};


class HtmlSearch
{
public:
	 HtmlSearchResult Search(const std::string& htmlContent);

private:
};

#endif  // HTML_SEARCH_HPP
