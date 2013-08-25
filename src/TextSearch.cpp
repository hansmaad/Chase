#include <cassert>
#include <stdexcept>
#include "TextSearch.hpp"


struct TextSearch
{
    unsigned lineNumber = 1;
    std::vector<TextSearchMatch> results;
    std::string::const_iterator cursor;
    const std::string* text;
    const std::string* searchFor;

    void Search(
            const std::string &text,
            const std::string &searchFor)
    {
        this->searchFor = &searchFor;
        this->text = &text;

        Search();
    }

    void Search()
    {
        if (searchFor->empty())
            throw std::runtime_error{"search text must not be empty"};

        ResetCursor();

        for(auto c : *text)
        {
            Handle(c);
        }
    }

    void ResetCursor()
    {
        cursor = begin(*searchFor);
    }

    void Handle(char c)
    {
        if (c == '\n')
        {
            ++lineNumber;
        }
        if (c == *cursor)
        {
            ++cursor;
        }
        else
        {
            ResetCursor();
        }
        if (IsMatch())
        {
            TextSearchMatch result;
            result.lineNumber = lineNumber;
            results.push_back(result);
            ResetCursor();
        }
    }

    bool IsMatch()
    {
        return cursor == end(*searchFor);
    }
};




std::vector<TextSearchMatch> SearchText(
        const std::string &text, const std::string &searchFor)
{
    TextSearch search;
    search.Search(text, searchFor);
    return std::move(search.results);
}






