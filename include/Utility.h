#pragma once

#include <cstring>
#include <string>
#include <vector>
#include <clang-c/Index.h>

inline bool operator ==(const CXCursor lhs, const CXCursor rhs)
{
    return memcmp(&lhs, &rhs, sizeof(CXCursor)) == 0;
}
inline bool operator !=(const CXCursor lhs, const CXCursor rhs)
{
    return memcmp(&lhs, &rhs, sizeof(CXCursor)) != 0;
}
inline bool operator <(const CXCursor lhs, const CXCursor rhs)
{
    return memcmp(&lhs, &rhs, sizeof(CXCursor)) < 0;
}

inline bool operator ==(const CXType lhs, const CXType rhs)
{
    return memcmp(&lhs, &rhs, sizeof(CXType)) == 0;
}
inline bool operator !=(const CXType lhs, const CXType rhs)
{
    return memcmp(&lhs, &rhs, sizeof(CXType)) != 0;
}
inline bool operator <(const CXType lhs, const CXType rhs)
{
    return memcmp(&lhs, &rhs, sizeof(CXType)) < 0;
}

namespace Utility
{

inline std::string ConvertString(const CXString & str)
{
    return clang_getCString(str);
}

inline std::string Indent(int indent)
{
    return std::string(4 * indent, ' ');
}

std::string Trim(const std::string & input);
void Split(const std::string & input, char delimiter, std::vector<std::string> & output);
void SplitPath(const std::string & path, std::string & directory, std::string & fileName, std::string & extension);

} // namespace Utility