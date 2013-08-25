#ifndef TEXTSEARCHANALYSIS_HPP
#define TEXTSEARCHANALYSIS_HPP

#include <string>
#include <vector>
#include "Analysis.hpp"
#include "TextSearch.hpp"

struct HttpResponse;

struct TextSearchResult
{
    std::string uri;
    std::vector<TextSearchMatch> matches;
};

class TextSearchObserver
{
public:
    virtual void NotifyMatch(const TextSearchResult& result) = 0;
};

class TextSearchAnalysis : public Analysis
{
public:
    void Run(const HttpResponse& response) override;

    void SetObserver(TextSearchObserver& observer)
    {
        this->observer = &observer;
    }

private:
    TextSearchObserver* observer;
};


#endif // TEXTSEARCHANALYSIS_HPP
