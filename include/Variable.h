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
    using Ptr = std::shared_ptr<VariableBase>;
    using List = std::vector<Ptr>;

    VariableBase() = delete;
    explicit VariableBase(Element::WeakPtr parent, SourceLocation sourceLocation, std::string name, AccessSpecifier accessSpecifier,
                          std::string type)
        : Declaration(std::move(parent), std::move(sourceLocation), std::move(name), accessSpecifier)
        , _type(std::move(type))
    {
    }

    const std::string & Type() const { return _type; }

private:
    std::string _type;
};

class Variable : public VariableBase
{
public:
    using Ptr = std::shared_ptr<Variable>;
    using List = std::vector<Ptr>;

    Variable() = delete;
    explicit Variable(Element::WeakPtr parent, SourceLocation sourceLocation, std::string name, AccessSpecifier accessSpecifier,
                      std::string type)
        : VariableBase(std::move(parent), std::move(sourceLocation), std::move(name), accessSpecifier, std::move(type))
    {
    }

    virtual std::string QualifiedDescription() const override { return QualifiedName(); }
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
};

class DataMember : public VariableBase
{
public:
    using Ptr = std::shared_ptr<DataMember>;
    using List = std::vector<Ptr>;

    DataMember() = delete;
    explicit DataMember(Element::WeakPtr parent, SourceLocation sourceLocation, std::string name, AccessSpecifier accessSpecifier,
                        std::string type)
        : VariableBase(std::move(parent), std::move(sourceLocation), std::move(name), accessSpecifier, std::move(type))
    {
    }

    virtual std::string QualifiedDescription() const override { return QualifiedName(); }
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
};

} // namespace CPPParser
