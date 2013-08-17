#ifndef HTTP_CLIENT_STUB_HPP
#define HTTP_CLIENT_STUB_HPP

#include <future>
#include <chrono>
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

    unsigned sleepMilliseconds = 0;

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
        auto& info = links[from];
        info.status = 200;
        info.links.emplace_back(std::move(to));
    }

    void AddRedirect(std::string from, std::string to, int status = 301)
    {
        auto& info = links[from];
        info.status = status;
        info.links.emplace_back(std::move(to));
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
    struct info
    {
        int status = 200;
        std::vector<std::string> links;
    };

    std::map<std::string, info> links;

    std::string MakeAnchor(const std::string& url)
    {
        auto a = "<a href=\"" + url + "\">link</a>";
        return a;
    }

    void PerformNext()
    {
        HttpResponse response;
        response.body = Html5Begin();
        response.uri = GetNextUrl();

        auto& info = links[response.uri];
        response.status = info.status;

        if (response.status == 200)
        {
            for(const auto& link : info.links)
            {
                response.body.append(MakeAnchor(link));
            };
        }
        else
        {
            response.header.insert(std::make_pair("Location", info.links.front()));
        }

        response.body.append(Html5End());

        if (sleepMilliseconds)
            std::this_thread::sleep_for(
                std::chrono::milliseconds(sleepMilliseconds));

         visits.push_back(response.uri);
        PushResponse(std::move(response));
    }
};


#endif  // HTTP_CLIENT_STUB_HPP
