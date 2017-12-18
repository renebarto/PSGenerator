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
    explicit Inheritance(Element::WeakPtr parent, SourceLocation sourceLocation, std::string name, AccessSpecifier accessSpecifier,
                         Element::WeakPtr base, bool isVirtual)
        : _name(std::move(name))
        , _parent(std::move(parent))
        , _base(std::move(base))
        , _accessSpecifier(accessSpecifier)
        , _isVirtual(isVirtual)
        , _sourceLocation(sourceLocation)
    {
    }

    const std::string & Name() const { return _name; }
    Element::Ptr Parent() const { return _parent.lock(); }
    AccessSpecifier Access() const { return _accessSpecifier; }
    Element::Ptr BaseType() const { return _base.lock(); }
    bool IsVirtual() const { return _isVirtual; }
    const SourceLocation & Location() const { return _sourceLocation; }

private:
    std::string _name;
    Element::WeakPtr _parent;
    Element::WeakPtr _base;
    AccessSpecifier _accessSpecifier;
    bool _isVirtual;
    SourceLocation _sourceLocation;
};

} // namespace CPPParser
