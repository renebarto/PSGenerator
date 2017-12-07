#pragma once

#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Declaration.h"

using namespace Utility;

namespace CPPParser
{

class Inheritance : public Declaration
{
public:
    Inheritance() = delete;
    explicit Inheritance(Declaration::WeakPtr parent, Declaration::WeakPtr base, AccessSpecifier accessSpecifier,
                         bool isVirtual)
        : Declaration(std::move(parent), std::move(base.lock()->Name()), accessSpecifier)
        , _isVirtual(isVirtual)
    {
    }
    explicit Inheritance(Declaration::WeakPtr parent, CXCursor token)
        : Declaration(std::move(parent), token)
        , _isVirtual(clang_isVirtualBase(token) != 0)
    {
    }

    bool IsVirtual() const { return _isVirtual; }

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
        stream << Indent(indent) << "Inheritance " << Name() << std::endl;;
    }
    virtual void GenerateCode(std::ostream & stream, int indent) const override
    {
        if (IsVirtual())
            stream << "virtual ";
        stream << Access() << " " << Name();
    }

private:
    bool _isVirtual;
};

} // namespace CPPParser
