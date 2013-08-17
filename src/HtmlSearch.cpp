#include "HtmlSearch.hpp"
#include <cctype>
#include <vector>


struct HtmlSearch::Impl
{
    enum class State
    {
        Default,
        Comment,
        ReadTagName,
        ReadTag,
        SkipTag,
        ReadAttribute,
        SkipAttribute,
        SkipComment,
    };

    HtmlSearchResult result;
    std::string buffer;
    State state;
    std::vector<AttachedSearch*> searches;

    HtmlSearchResult Search(const std::string& htmlContent)
    {
        ResetSearches();
        state = State::Default;
        for(auto c : htmlContent)
        {
            HandleBySearches(c);
            Handle(c);
        }

        return result;
    }

    void ResetSearches()
    {
        for(auto&& search : searches)
            search->Reset();
    }

    void HandleBySearches(char c)
    {
        for(auto&& search : searches)
            search->Handle(c);
    }

    void Handle(char c)
    {
        switch(state)
        {
        case State::Default:
            ReadDefault(c);
            break;
        case State::ReadTagName:
            ReadTagName(c);
            break;
        case State::SkipTag:
            SkipTag(c);
            break;
        case State::ReadTag:
            ReadTag(c);
            break;
        case State::ReadAttribute:
            ReadAttribute(c);
            break;
        case State::SkipAttribute:
            SkipAttribute(c);
            break;
        case State::SkipComment:
            SkipComment(c);
            break;
        }
    }

    void ReadDefault(char c)
    {
        if (c == '<')
            state = State::ReadTagName;
    }

    void ReadTagName(char c)
    {
        if (IsWhitespace(c))
        {
            if (buffer == "a")
                state = State::ReadTag;
            else if (buffer == "!--")
                state = State::SkipComment;
            else
                state = State::SkipTag;
            ClearBuffer();
        }
        else if (c == '>')
        {
            state = State::Default;
            ClearBuffer();
        }
        else
            StoreToLower(c);
    }

    void SkipTag(char c)
    {
        if (c == '>')
            state = State::Default;
    }

    void ReadTag(char c)
    {
        if (c == '=')
        {
            if (buffer == "href")
                state = State::ReadAttribute;
            else
                state = State::SkipAttribute;
            ClearBuffer();
        }
        else if (!IsWhitespace(c))
            StoreToLower(c);
    }

    void ReadAttribute(char c)
    {
        if (buffer.empty())
        {
            if (!IsWhitespace(c))
            {
                if (c == '"' || c == '\'')
                    Store(c);
                else
                {
                    state = State::SkipTag;
                }
            }
        }
        else if (c == buffer.front())
        {
            StoreLink();
            ClearBuffer();
            state = State::SkipTag;
        }
        else
        {
            Store(c);
        }
    }

    void SkipAttribute(char c)
    {
        if (c == '"')
        {
            if (buffer == "\"")
            {
                state = State::ReadTag;
                ClearBuffer();
            }
            else
                Store(c);
        }
    }

    void SkipComment(char c)
    {
        if (c == '>')
        {
            if (buffer.size() > 1)
            {
                state = State::Default;
            }
            ClearBuffer();
        }
        else if (c == '-')
        {
            buffer.push_back(c);
        }
    }

    void StoreToLower(char c)
    {
        Store(std::tolower(c));
    }

    void Store(char c)
    {
        buffer.push_back(c);
    }

    void ClearBuffer()
    {
        buffer.clear();
    }

    void StoreLink()
    {
        // trim leading quote from buffer
        result.links.emplace_back(buffer, 1);
    }

    bool IsWhitespace(char c)
    {
        return std::isspace(c) != 0;
    }
};


HtmlSearch::HtmlSearch()
    : impl{new Impl{}}
{

}

HtmlSearch::~HtmlSearch() = default;

HtmlSearchResult HtmlSearch::Search(const std::string& htmlContent)
{
    return impl->Search(htmlContent);
}

void HtmlSearch::AttachSearch(AttachedSearch *search)
{
    impl->searches.push_back(search);
}


