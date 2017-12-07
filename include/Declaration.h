#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <clang-c/Index.h>
#include "include/Utility.h"

using namespace Utility;

namespace CPPParser
{

class Namespace;
class Class;
class Struct;
class IASTVisitor;

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

class Declaration
    : public std::enable_shared_from_this<Declaration>
{
public:
    using WeakPtr = std::weak_ptr<Declaration>;
    using Ptr = std::shared_ptr<Declaration>;

    Declaration() = delete;
    explicit Declaration(WeakPtr parent, std::string name, AccessSpecifier accessSpecifier)
        : _name(std::move(name))
        , _parent(std::move(parent))
        , _accessSpecifier(accessSpecifier)
    {
    }
    explicit Declaration(WeakPtr parent, CXCursor token)
        : _name(ConvertString(clang_getCursorSpelling(token)))
        , _parent(std::move(parent))
        , _accessSpecifier(ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token)))
    {
    }
    virtual ~Declaration() = default;

    const std::string & Name() const { return _name; }
    std::shared_ptr<Declaration> Parent() const { return _parent.lock(); }
    AccessSpecifier Access() const { return _accessSpecifier; }

    virtual bool Visit(IASTVisitor & visitor) const = 0;
    virtual bool IsValid() const { return false; }
    virtual void Show(std::ostream & stream, int indent) const = 0;
    virtual void GenerateCode(std::ostream & stream, int indent) const = 0;

private:
    std::string _name;
    std::weak_ptr<Declaration> _parent;
    AccessSpecifier _accessSpecifier;
};

} // namespace CPPParser
