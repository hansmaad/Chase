#ifndef TEXTSEARCH_HPP
#define TEXTSEARCH_HPP

#include <vector>
#include <string>

#include "Analysis.hpp"


struct TextSearchMatch
{
    int lineNumber;
};


std::vector<TextSearchMatch> SearchText(
        const std::string& text, const std::string& searchFor);



#endif // TEXTSEARCH_HPP
