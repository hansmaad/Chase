#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <map>

struct HttpResponse
{
    std::string uri, body;
    std::multimap<std::string, std::string> header;
    unsigned status;

    const std::string& ContentType() const
    {
        static const auto unknown = std::string{"unknown"};
        return GetHeader("Content-Type", unknown);
    }

    const std::string& Location() const
    {
        static const std::string unknown;
        return GetHeader("Location", unknown);
    }

private:
    template<typename S>
    const std::string& GetHeader(S key, const std::string& defaultValue) const
    {
        auto it = header.find(key);
        return it == end(header) ? defaultValue : it->second;
    }
};


#endif // HTTPRESPONSE_HPP
