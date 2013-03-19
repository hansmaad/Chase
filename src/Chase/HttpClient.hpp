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

    virtual void Start(
        InputQueue& urlQueue, 
        ResponseQueue& responseQueue) = 0;
};

#endif  // HTTP_CLIENT_HPP
