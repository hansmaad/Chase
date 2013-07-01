#include "SingleHttpClient.hpp"
#include <curl/curl.h>

namespace
{

struct GlobalInit
{
    GlobalInit()
    {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    ~GlobalInit()
    {
        curl_global_cleanup();
    }
};

inline void* InitHandle()
{
    static GlobalInit initializer;
    return curl_easy_init();
}

inline void Cleanup(void* handle)
{
    curl_easy_cleanup(handle);
}


struct Content
{
    std::string data;
    static size_t Write(char * data, size_t size, size_t nmemb, void * p)
    {
        return static_cast<Content*>(p)->WriteImpl(data, size, nmemb);
    }

    size_t WriteImpl(char* ptr, size_t size, size_t nmemb)
    {
        data.insert(end(data), ptr, ptr + size * nmemb);
        return size * nmemb;
    }
};

}

SingleHttpClient::SingleHttpClient()
    : handle(InitHandle())
{
}

SingleHttpClient::~SingleHttpClient()
{
    Cleanup(handle);
}


HttpResponse SingleHttpClient::Get(std::string uri) const
{
    Content content;

    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &content);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &Content::Write);
    curl_easy_setopt(handle, CURLOPT_URL, uri.c_str());
    curl_easy_perform(handle);
    HttpResponse response;
    response.uri = move(uri);
    response.body = move(content.data);
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response.status);

    return response;
}
