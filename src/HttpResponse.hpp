#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>

struct HttpResponse
{
    std::string uri, header, body, contentType;
    unsigned status;
};


#endif // HTTPRESPONSE_HPP
