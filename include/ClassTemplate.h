#pragma once

#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Object.h"

using namespace Utility;

namespace CPPParser
{

class ClassTemplate : public Object
{
public:
    using Ptr = std::shared_ptr<ClassTemplate>;
    using List = std::vector<Ptr>;

    ClassTemplate() = delete;
    explicit ClassTemplate(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier)
        : Object(std::move(parent), std::move(name), accessSpecifier, AccessSpecifier::Private)
        , _templateParameters()
    {}

    const std::vector<std::string> & TemplateParameters() const { return _templateParameters; }

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

    void AddTemplateParameter(std::string name)
    {
        _templateParameters.push_back(std::move(name));
    }

private:
    std::vector<std::string> _templateParameters;
};

} // namespace CPPParser
