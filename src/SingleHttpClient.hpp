#ifndef SINGLEHTTPCLIENT_HPP
#define SINGLEHTTPCLIENT_HPP

#include "HttpResponse.hpp"


class SingleHttpClient
{
public:
    SingleHttpClient();
    ~SingleHttpClient();

    HttpResponse Get(std::string uri) const;
private:
    void* handle;
};

#endif // SINGLEHTTPCLIENT_HPP
