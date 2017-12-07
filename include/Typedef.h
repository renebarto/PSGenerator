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
    explicit Typedef(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier,
                     std::string type)
        : Declaration(std::move(parent), std::move(name), accessSpecifier)
        , _type(std::move(type))
    {
    }
    explicit Typedef(Declaration::WeakPtr parent, CXCursor token)
        : Declaration(std::move(parent), token)
        , _type(ConvertString(clang_getTypeSpelling(clang_getTypedefDeclUnderlyingType(token))))
    {
    }
    const std::string & Type() const { return _type; }

    virtual bool Visit(IASTVisitor & visitor) const override
    {
        bool ok = true;
        if (!visitor.Enter(*this))
            ok = false;
        if (!visitor.Leave(*this))
            ok = false;
        return ok;
    }
    virtual void Show(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << "Typedef " << Name() << " : " << Type() << std::endl;
    }
    virtual void GenerateCode(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << "typedef " << Name() << " " << Type() << ";" << std::endl;
    }

private:
    std::string _type;
};

} // namespace CPPParser
