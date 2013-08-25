#ifndef SEARCH_HPP
#define SEARCH_HPP

struct HttpResponse;

class Analysis
{
public:
    virtual void Run(const HttpResponse& response) = 0;
};


#endif // SEARCH_HPP
