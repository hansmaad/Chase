#include "ExternalLinkFilter.hpp"
#include <network/uri.hpp>


bool ExternalLinkFilter::ShouldFollowLink(
    const network::uri &base, const network::uri &link) const
{
    return base.host() == link.host();
}
