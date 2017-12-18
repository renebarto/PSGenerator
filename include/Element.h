#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <clang-c/Index.h>
#include "include/Utility.h"

using namespace Utility;

namespace CPPParser
{

class IASTVisitor;

class Element
    : public std::enable_shared_from_this<Element>
{
public:
    using WeakPtr = std::weak_ptr<Element>;
    using Ptr = std::shared_ptr<Element>;

    Element() = delete;
    explicit Element(WeakPtr parent, SourceLocation sourceLocation, std::string name, AccessSpecifier accessSpecifier)
        : _name(std::move(name))
        , _parent(std::move(parent))
        , _accessSpecifier(accessSpecifier)
        , _sourceLocation(sourceLocation)
    {
    }
    virtual ~Element() = default;

    const std::string & Name() const { return _name; }
    Ptr Parent() const { return _parent.lock(); }
    AccessSpecifier Access() const { return _accessSpecifier; }
    const SourceLocation & Location() const { return _sourceLocation; }

    virtual std::string QualifiedDescription() const = 0;
    virtual std::string QualifiedName() const = 0;
    virtual bool TraverseBegin(IASTVisitor & visitor) const = 0;
    virtual bool TraverseEnd(IASTVisitor & visitor) const = 0;
    virtual bool Visit(IASTVisitor & visitor) const = 0;
    virtual bool IsValid() const { return false; }

private:
    std::string _name;
    WeakPtr _parent;
    AccessSpecifier _accessSpecifier;
    SourceLocation _sourceLocation;
};

} // namespace CPPParser
