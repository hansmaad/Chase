#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include "BlockingQueue.hpp"

struct HttpResponse
{
    std::string header, body;
};

class HttpClient
{
public:
    typedef BlockingQueue<std::string> InputQueue;
    typedef BlockingQueue<HttpResponse> ResponseQueue;

    virtual void StartAsync(
        InputQueue& urlQueue, 
        ResponseQueue& responseQueue) = 0;

    virtual void Wait() = 0;

    virtual ~HttpClient() {}
};

#endif  // HTTP_CLIENT_HPP
