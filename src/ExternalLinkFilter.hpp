#ifndef EXTERNALLINKFILTER_HPP
#define EXTERNALLINKFILTER_HPP

#include "LinkFilter.hpp"


class ExternalLinkFilter : public LinkFilter
{
public:
    bool ShouldFollowLink(const network::uri &base,
                          const network::uri &link) const override;
};

#endif // EXTERNALLINKFILTER_HPP
