#include <cassert>
#include <stdexcept>
#include "TextSearch.hpp"


void TextSearch::Handle(char c)
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
    if (cursor == end(searchText))
    {
        TextSearchResult result;
        result.lineNumber = lineNumber;
        results.push_back(result);
        ResetCursor();
    }
}

void TextSearch::ResetCursor()
{
    cursor = begin(searchText);
}

void TextSearch::Reset()
{
    results.clear();
    lineNumber = 1;
    assert(!searchText.empty());
    ResetCursor();
}

void TextSearch::SearchFor(std::string searchText)
{
    if (searchText.empty())
        throw std::runtime_error{"search text must not be empty"};
    this->searchText = std::move(searchText);
    ResetCursor();
}




