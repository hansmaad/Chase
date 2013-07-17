#ifndef CRAWLEROBSERVER_HPP
#define CRAWLEROBSERVER_HPP


struct HttpResponse;

enum class ErrorCode
{
    InvalidUri
};

class CrawlerObserver
{
public:
    virtual void NotifyResponse(const HttpResponse& response) { }

    virtual void NotifyError(ErrorCode error, std::string content) { }
};

#endif // CRAWLEROBSERVER_HPP
