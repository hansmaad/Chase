#ifndef LINKFILTER_HPP
#define LINKFILTER_HPP

namespace network
{
    class uri;
}

class LinkFilter
{
public:
    virtual bool ShouldFollowLink(const network::uri& base,
                                  const network::uri& link) const = 0;
};

#endif // LINKFILTER_HPP
