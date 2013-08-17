#ifndef STATUS_HPP
#define STATUS_HPP

namespace Status
{

inline bool IsOk(int status)
{
    return status == 200;
}

inline bool IsRedirect(int status)
{
    return status == 300 ||
           status == 301 ||
           status == 302 ||
           status == 303 ||
           status == 307;
}

}

#endif // STATUS_HPP
