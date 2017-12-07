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
    explicit Namespace(Declaration::WeakPtr parent, CXCursor token)
        : Container(std::move(parent), token)
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
    virtual void Show(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << "Namespace " << Name() << " {" << std::endl;
        ShowContents(stream, indent + 1);
        stream << Indent(indent) << "}" << std::endl;
    }
    virtual void GenerateCode(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << "namespace " << (Name().empty() ? "" : Name() + " ") << "{" << std::endl;
        GenerateCodeContents(stream, indent + 1);
        stream << Indent(indent) << "} // namespace " << (Name().empty() ? "<anonymous>" : Name()) << std::endl;
    }
};

} // namespace CPPParser
