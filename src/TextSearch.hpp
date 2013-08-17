#ifndef TEXTSEARCH_HPP
#define TEXTSEARCH_HPP

#include <vector>
#include <string>
#include "AttachedSearch.hpp"


struct TextSearchResult
{
    int lineNumber;
};

class TextSearch : public AttachedSearch
{
public:
    void Handle(char c) override;

    void Reset() override;

    void SearchFor(std::string searchText);

    const std::vector<TextSearchResult> Results() const
    {
        return results;
    }

private:
    void ResetCursor();

    std::vector<TextSearchResult> results;
    std::string searchText;
    std::string::const_iterator cursor;
    unsigned lineNumber = 1;
};

#endif // TEXTSEARCH_HPP
