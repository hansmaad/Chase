#ifndef SINGLEHTTPCLIENT_HPP
#define SINGLEHTTPCLIENT_HPP

#include "HttpResponse.hpp"


class SingleHttpClient
{
public:
    HttpResponse Get(const std::string& uri) const;

private:
    void* handle;
};

#endif // SINGLEHTTPCLIENT_HPP
