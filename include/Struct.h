#pragma once

#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Object.h"

using namespace Utility;

namespace CPPParser
{

class Struct : public Object
{
public:
    using Ptr = std::shared_ptr<Struct>;
    using List = std::vector<Ptr>;

    Struct() = delete;
    explicit Struct(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier)
        : Object(std::move(parent), std::move(name), accessSpecifier, AccessSpecifier::Public)
    {}

    virtual bool TraverseBegin(IASTVisitor & visitor) const override
    {
        return visitor.Enter(*this);
    }
    virtual bool TraverseEnd(IASTVisitor & visitor) const override
    {
        return visitor.Leave(*this);
    }
    virtual bool Visit(IASTVisitor & visitor) const override
    {
        bool ok = true;
        if (!visitor.Enter(*this))
            ok = false;
        if (!VisitChildren(visitor))
            ok = false;
        if (!visitor.Leave(*this))
            ok = false;
        return ok;
    }
};

} // namespace CPPParser
