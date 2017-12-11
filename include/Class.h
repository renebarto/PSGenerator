#pragma once

#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Object.h"

using namespace Utility;

namespace CPPParser
{

class Class : public Object
{
public:
    using Ptr = std::shared_ptr<Class>;
    using List = std::vector<Ptr>;

    Class() = delete;
    explicit Class(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier)
        : Object(std::move(parent), std::move(name), accessSpecifier, AccessSpecifier::Private)
    {}

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
