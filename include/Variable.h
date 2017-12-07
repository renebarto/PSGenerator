#pragma once

#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Declaration.h"

using namespace Utility;

namespace CPPParser
{

class VariableBase : public Declaration
{
public:
    VariableBase() = delete;
    explicit VariableBase(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier,
                      std::string type)
        : Declaration(std::move(parent), std::move(name), accessSpecifier)
        , _type(std::move(type))
    {
    }
    explicit VariableBase(Declaration::WeakPtr parent, CXCursor token)
        : Declaration(std::move(parent), token)
        , _type(ConvertString(clang_getTypeSpelling(clang_getCursorType(token))))
    {
    }

    const std::string & Type() const { return _type; }

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

class Variable : public VariableBase
{
public:
    Variable() = delete;
    explicit Variable(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier,
                      std::string type)
        : VariableBase(std::move(parent), std::move(name), accessSpecifier, std::move(type))
    {
    }
    explicit Variable(Declaration::WeakPtr parent, CXCursor token)
        : VariableBase(std::move(parent), token)
    {
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
    virtual void Show(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << "Typedef " << Name() << " : " << Type() << std::endl;
    }
    virtual void GenerateCode(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << "typedef " << Name() << " " << Type() << ";" << std::endl;
    }
};

class DataMember : public VariableBase
{
public:
    DataMember() = delete;
    explicit DataMember(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier,
                        std::string type)
        : VariableBase(std::move(parent), std::move(name), accessSpecifier, std::move(type))
    {
    }
    explicit DataMember(Declaration::WeakPtr parent, CXCursor token)
        : VariableBase(std::move(parent), token)
    {
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
    virtual void Show(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << "DataMember " << Name() << " : " << Type() << std::endl;
    }
    virtual void GenerateCode(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << Type() << " " << Name() << ";" << std::endl;
    }
};

} // namespace CPPParser
