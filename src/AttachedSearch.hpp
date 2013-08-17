#ifndef SEARCH_HPP
#define SEARCH_HPP


class AttachedSearch
{
public:
    virtual void Reset() = 0;
    virtual void Handle(char c) = 0;
};


#endif // SEARCH_HPP
