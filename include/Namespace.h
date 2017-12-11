#pragma once

#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Container.h"
#include "include/IASTVisitor.h"

using namespace Utility;

namespace CPPParser
{

class Namespace : public Container
{
public:
    using Ptr = std::shared_ptr<Namespace>;
    using List = std::vector<Ptr>;

    Namespace() = delete;
    explicit Namespace(Declaration::WeakPtr parent, std::string name)
        : Container(std::move(parent), std::move(name), AccessSpecifier::Invalid)
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
