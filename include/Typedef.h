#pragma once

#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Declaration.h"

using namespace Utility;

namespace CPPParser
{

class Typedef : public Declaration
{
public:
    using Ptr = std::shared_ptr<Typedef>;
    using List = std::vector<Ptr>;

    Typedef() = delete;
    explicit Typedef(Element::WeakPtr parent, SourceLocation sourceLocation, std::string name, AccessSpecifier accessSpecifier,
                     std::string type)
        : Declaration(std::move(parent), std::move(sourceLocation), std::move(name), accessSpecifier)
        , _type(std::move(type))
    {
    }

    const std::string & Type() const { return _type; }

    virtual std::string QualifiedDescription() const override { return "typedef " + QualifiedName(); }
    virtual std::string QualifiedName() const override
    {
        std::string result = Type() + " ";
        if (Parent() != nullptr)
        {
            result += Parent()->QualifiedName() + "::";
        }
        result += Name();
        return result;
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
        if (!visitor.Leave(*this))
            ok = false;
        return ok;
    }

private:
    std::string _type;
};

} // namespace CPPParser
