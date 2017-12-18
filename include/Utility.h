#pragma once

#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <clang-c/Index.h>

inline void TRACE(const std::string & function, const std::string & text)
{
    std::cerr << function << "(" << text << ")" << std::endl;
}

inline void TRACE2(const std::string & function, const std::string & text, const std::string & name)
{
    std::cerr << function << "(" << text << ") : " << name << std::endl;
}

inline std::ostream & operator << (std::ostream & stream, const CXString & str)
{
    stream << clang_getCString(str);
    return stream;
}

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

enum class AccessSpecifier
{
    Invalid,
    Public,
    Protected,
    Private,
};

inline AccessSpecifier ConvertAccessSpecifier(CX_CXXAccessSpecifier specifier)
{
    switch (specifier)
    {
        case CX_CXXAccessSpecifier::CX_CXXPrivate: return AccessSpecifier::Private;
        case CX_CXXAccessSpecifier::CX_CXXProtected: return AccessSpecifier::Protected;
        case CX_CXXAccessSpecifier::CX_CXXPublic: return AccessSpecifier::Public;
    }
    return AccessSpecifier::Invalid;
}

inline std::ostream & operator << (std::ostream & stream, AccessSpecifier specifier)
{
    switch (specifier)
    {
        case AccessSpecifier::Private:
            stream << "private"; break;
        case AccessSpecifier::Protected:
            stream << "protected"; break;
        case AccessSpecifier::Public:
            stream << "public"; break;
    }
    return stream;
}

enum class IncludeSpecifier
{
    System,
    Local,
};

inline std::ostream & operator << (std::ostream & stream, IncludeSpecifier specifier)
{
    switch (specifier)
    {
        case IncludeSpecifier::System:
            stream << "system"; break;
        case IncludeSpecifier::Local:
            stream << "local"; break;
    }
    return stream;
}

namespace Utility
{

inline std::string ConvertString(const CXString & str)
{
    return clang_getCString(str);
}

inline std::string Indent(int indent)
{
    return std::string(4 * ((indent < 0) ? 0 : indent), ' ');
}

std::string Trim(const std::string & input);
void Split(const std::string & input, char delimiter, std::vector<std::string> & output);
void SplitPath(const std::string & path, std::string & directory, std::string & fileName, std::string & extension);

struct SourceLocation
{
    SourceLocation()
        : fileName()
        , line()
        , column()
        , fileOffset()
    {}
    SourceLocation(CXCursor token)
        : fileName()
        , line()
        , column()
        , fileOffset()
    {
        CXSourceLocation location = clang_getCursorLocation(token);
        CXFile file;
        clang_getSpellingLocation(location, &file, &line, &column, &fileOffset);
        fileName = ConvertString(clang_getFileName(file));
    }
    std::string fileName;
    unsigned line;
    unsigned column;
    unsigned fileOffset;
};

inline std::ostream & operator << (std::ostream & stream, const SourceLocation & location)
{
    stream << location.fileName << ":" << location.line << ":" << location.column
           << "-" << location.fileOffset << std::endl;
}

} // namespace Utility