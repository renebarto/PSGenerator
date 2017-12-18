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
    explicit Namespace(Element::WeakPtr parent, SourceLocation sourceLocation, std::string name)
        : Container(std::move(parent), std::move(sourceLocation), std::move(name), AccessSpecifier::Invalid)
    {}

    virtual std::string QualifiedDescription() const override { return "namespace " + QualifiedName(); }
    virtual std::string QualifiedName() const override
    {
        std::string result;
        if (Parent() != nullptr)
        {
            result = Parent()->QualifiedName() + "::";
        }
        return result + (Name().empty() ? "<anonymous>" : Name());
    }
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
