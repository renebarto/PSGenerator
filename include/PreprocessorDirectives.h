#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <clang-c/Index.h>
#include "include/Container.h"

using namespace Utility;

namespace CPPParser
{

class IncludeDirective
    : public Element
{
public:
    using Ptr = std::shared_ptr<IncludeDirective>;
    using List = std::vector<Ptr>;

    IncludeDirective() = delete;
    explicit IncludeDirective(WeakPtr parent, SourceLocation sourceLocation, std::string name, IncludeSpecifier includeSpecifier)
        : Element(std::move(parent), std::move(sourceLocation), std::move(name), AccessSpecifier::Invalid)
        , _includeSpecifier(includeSpecifier)
    {
    }

    IncludeSpecifier IncludeType() const { return _includeSpecifier; }

    virtual std::string QualifiedDescription() const override { return "include " + QualifiedName(); }
    virtual std::string QualifiedName() const override
    {
        std::string result;
        if (Parent() != nullptr)
        {
            result = Parent()->QualifiedName() + "::";
        }
        return result + Name();
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
    IncludeSpecifier _includeSpecifier;
};

class IfdefDirective
    : public Container
{
public:
    using Ptr = std::shared_ptr<IfdefDirective>;
    using List = std::vector<Ptr>;

    IfdefDirective() = delete;
    explicit IfdefDirective(WeakPtr parent, SourceLocation sourceLocation, std::string name)
        : Container(std::move(parent), std::move(sourceLocation), std::move(name), AccessSpecifier::Invalid)
    {
    }

    virtual std::string QualifiedDescription() const override { return "ifdef " + QualifiedName(); }
    virtual std::string QualifiedName() const override
    {
        std::string result;
        if (Parent() != nullptr)
        {
            result = Parent()->QualifiedName() + "::";
        }
        return result + Name();
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
};

class IfDirective
    : public Container
{
public:
    using Ptr = std::shared_ptr<IfDirective>;
    using List = std::vector<Ptr>;

    IfDirective() = delete;
    explicit IfDirective(WeakPtr parent, SourceLocation sourceLocation, std::string name)
        : Container(std::move(parent), std::move(sourceLocation), std::move(name), AccessSpecifier::Invalid)
    {
    }

    virtual std::string QualifiedDescription() const override { return "if " + QualifiedName(); }
    virtual std::string QualifiedName() const override
    {
        std::string result;
        if (Parent() != nullptr)
        {
            result = Parent()->QualifiedName() + "::";
        }
        return result + Name();
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
};

class DefineDirective
    : public Element
{
public:
    using Ptr = std::shared_ptr<DefineDirective>;
    using List = std::vector<Ptr>;

    DefineDirective() = delete;
    explicit DefineDirective(WeakPtr parent, SourceLocation sourceLocation, std::string name)
        : Element(std::move(parent), std::move(sourceLocation), std::move(name), AccessSpecifier::Invalid)
    {
    }

    virtual std::string QualifiedDescription() const override { return "ifdef " + QualifiedName(); }
    virtual std::string QualifiedName() const override
    {
        std::string result;
        if (Parent() != nullptr)
        {
            result = Parent()->QualifiedName() + "::";
        }
        return result + Name();
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
};

class UndefDirective
    : public Element
{
public:
    using Ptr = std::shared_ptr<UndefDirective>;
    using List = std::vector<Ptr>;

    UndefDirective() = delete;
    explicit UndefDirective(WeakPtr parent, SourceLocation sourceLocation, std::string name)
        : Element(std::move(parent), std::move(sourceLocation), std::move(name), AccessSpecifier::Invalid)
    {
    }

    virtual std::string QualifiedDescription() const override { return "ifdef " + QualifiedName(); }
    virtual std::string QualifiedName() const override
    {
        std::string result;
        if (Parent() != nullptr)
        {
            result = Parent()->QualifiedName() + "::";
        }
        return result + Name();
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
};

} // namespace CPPParser
