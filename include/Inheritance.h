#pragma once

#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Declaration.h"

using namespace Utility;

namespace CPPParser
{

class Inheritance
{
public:
    using Ptr = std::shared_ptr<Inheritance>;

    Inheritance() = delete;
    explicit Inheritance(Declaration::WeakPtr parent, Declaration::WeakPtr base, AccessSpecifier accessSpecifier,
                         bool isVirtual)
        : _name(std::move(base.lock()->Name()))
        , _parent(std::move(parent))
        , _accessSpecifier(accessSpecifier)
        , _isVirtual(isVirtual)
    {
    }
    explicit Inheritance(Declaration::WeakPtr parent, CXCursor token)
        : _name(ConvertString(clang_getCursorSpelling(token)))
        , _parent(std::move(parent))
        , _accessSpecifier(ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token)))
        , _isVirtual(clang_isVirtualBase(token) != 0)
    {
    }

    const std::string & Name() const { return _name; }
    std::shared_ptr<Declaration> Parent() const { return _parent.lock(); }
    AccessSpecifier Access() const { return _accessSpecifier; }
    bool IsVirtual() const { return _isVirtual; }

    void Show(std::ostream & stream, int indent) const
    {
        stream << Indent(indent) << "Inheritance " << Name() << std::endl;;
    }
    void GenerateCode(std::ostream & stream, int indent) const
    {
        if (IsVirtual())
            stream << "virtual ";
        stream << Access() << " " << Name();
    }

private:
    std::string _name;
    std::weak_ptr<Declaration> _parent;
    AccessSpecifier _accessSpecifier;
    bool _isVirtual;
};

} // namespace CPPParser
