#ifndef CRAWLEROBSERVER_HPP
#define CRAWLEROBSERVER_HPP


struct HttpResponse;

class CrawlerObserver
{
public:
    virtual void NotifyResponse(const HttpResponse& response) = 0;

};

#endif // CRAWLEROBSERVER_HPP
