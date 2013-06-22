#ifndef HTTP_CLIENT_STUB_HPP
#define HTTP_CLIENT_STUB_HPP

#include <future>
#include <algorithm>
#include <map>
#include "HtmlTestContent.hpp"
#include "HttpClient.hpp"


class HttpClientBase : public HttpClient
{
public:
    void StartAsync(InputQueue& urlQueue, ResponseQueue& responseQueue) override
    {
        this->urlQueue = &urlQueue;
        this->responseQueue = &responseQueue;
        thread = std::async(std::launch::async, [this]() { Run(); });
    }

    void Wait() override
    {
        thread.wait();
    }

protected:
    virtual void Run() = 0;

    std::string GetNextUrl()
    {
        return urlQueue->Pop();
    }

    void PushResponse(HttpResponse&& response)
    {
        responseQueue->Push(std::move(response));
    }
    
    std::future<void> thread;
    InputQueue* urlQueue;
    ResponseQueue* responseQueue;
};


class HttpClientStub : public HttpClientBase
{
public:
    void AddLink(std::string from, std::string to)
    {
        //links.emplace(std::move(from), std::move(to));
        links.insert(std::make_pair(std::move(from), std::move(to)));
    }

    std::vector<std::string> visits;

protected:
    void Run() override
    {
        for(;;)
        {
            try
            {
                PerformNext();
            }
            catch(BlockingQueueInterruptedException)
            {
                break;
            }
        }
    }

private:
    std::multimap<std::string, std::string> links;

    std::string MakeAnchor(const std::string& url)
    {
        std::string a = "<a href=\"" + url + "\">link</a>";
        return a;
    }

    void PerformNext()
    {
        HttpResponse response;
        response.body = Html5Begin();
        response.uri = GetNextUrl();

        //auto nextUrl = GetNextUrl();
        visits.push_back(response.uri);


        auto linksRange = links.equal_range(response.uri);
        for(auto it = linksRange.first; it != linksRange.second; ++it)
        {
            response.body.append(MakeAnchor(it->second));
        };

        response.body.append(Html5End());
        PushResponse(std::move(response));
    }
};


#endif  // HTTP_CLIENT_STUB_HPP
