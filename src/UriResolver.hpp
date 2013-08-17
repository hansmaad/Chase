#ifndef URIRESOLVER_HPP
#define URIRESOLVER_HPP

#include <network/uri.hpp>

class UriResolver
{
public:
    typedef network::uri Uri;

    network::uri Resolve(const Uri& base, const Uri& link);
};

#endif // URIRESOLVER_HPP
