#include "TextSearchAnalysis.hpp"
#include "HttpResponse.hpp"
#include "TextSearch.hpp"

void TextSearchAnalysis::Run(const HttpResponse &response)
{
    auto results = SearchText(response.body, searchFor);
    if (!results.empty())
    {
        TextSearchResult searchResult;
        searchResult.uri = response.uri;
        searchResult.matches = std::move(results);
        if (observer)
            observer->NotifyMatch(searchResult);
    }
}

#include "TextSearchAnalysis.hpp"
