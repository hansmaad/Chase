#include "UriResolver.hpp"
#include "network/uri/uri_builder.hpp"

namespace
{

inline network::uri StripFragment(const network::uri& uri)
{
    network::uri_builder builder;
    builder.scheme(std::string(*uri.scheme()));
    builder.host(std::string(*uri.host()));
    if (uri.path())
        builder.path(std::string(*uri.path()));
    if (uri.port())
        builder.port(std::string(*uri.port()));
    if (uri.query())
        builder.query(std::string(*uri.query()));
    return network::uri{builder};
}

}


UriResolver::Uri UriResolver::Resolve(
    const UriResolver::Uri &base, const UriResolver::Uri &link)
{
    auto resolved = base.resolve(
                link,
                network::uri_comparison_level::string_comparison);
    if (resolved.fragment())
    {
        return StripFragment(resolved);
    }
    return resolved;
}
