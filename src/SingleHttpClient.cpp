#include "SingleHttpClient.hpp"

#include <network/http/client.hpp>
#include <network/http/request.hpp>
#include <network/http/response.hpp>

namespace
{

inline bool BeginsWith(const std::string& text, const std::string& substr)
{
    return text.compare(0, substr.length(), substr) == 0;
}

inline bool ShouldLoadBodyOfType(const std::string& contentType)
{
    static const std::string htmlType = "text/html";
    return BeginsWith(contentType, htmlType);
}

}

HttpResponse SingleHttpClient::Get(const std::string& uri) const
{
    using namespace network;

    HttpResponse response;
    response.uri = move(uri);

    http::client httpClient;
    auto request = http::client::request{uri};
    request << header("Connection", "close");

    auto httpHeadResponse = httpClient.head(request);
    response.status = status(httpHeadResponse);

    std::multimap<std::string, std::string> head = headers(httpHeadResponse);
    auto it = head.find("Content-Type");
    response.contentType = it == end(head) ? "unknown" : it->second;

    if (ShouldLoadBodyOfType(response.contentType))
    {
        auto httpResponse = httpClient.get(request);
        response.body = body(httpResponse);
    }

    return response;
}

