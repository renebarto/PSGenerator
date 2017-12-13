#pragma once

#include <vector>
#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Declaration.h"

using namespace std;
using namespace Utility;

namespace CPPParser
{

class Parameter
{
public:
    using Ptr = std::shared_ptr<Parameter>;
    using List = std::vector<Ptr>;

    Parameter() = delete;
    explicit Parameter(std::string name, std:: string type)
        : _name(std::move(name))
        , _type(std::move(type))
    {}
    const std::string & Name() const { return _name; }
    const std::string & Type() const { return _type; }

private:
    std::string _name;
    std::string _type;
};

using ParameterList = std::vector<Parameter>;

enum FunctionFlags : uint16_t
{
    None = 0x0000,
    Const = 0x0001,
    Static = 0x0002,
    Virtual = 0x0004,
    PureVirtual = 0x0008,
    Override = 0x0010,
    Final = 0x0020,
    Default = 0x0040,
    Delete = 0x0080,
    Inline = 0x0100,
};

class FunctionBase : public Declaration
{
public:
    using Ptr = std::shared_ptr<FunctionBase>;
    using List = std::vector<Ptr>;

    FunctionBase() = delete;
    explicit FunctionBase(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier,
                          std::string type, ParameterList parameters,
                          FunctionFlags flags)
        : Declaration(std::move(parent), std::move(name), accessSpecifier)
          , _type(std::move(type))
          , _parameters(std::move(parameters))
          , _flags(flags)
    {
        if (_flags & FunctionFlags::PureVirtual)
            _flags = static_cast<FunctionFlags>(_flags | FunctionFlags::Virtual);
    }
    const std::string & Type() const { return _type; }
    const ParameterList & Parameters() const { return _parameters; }
    bool IsConst() const { return (_flags & FunctionFlags::Const) != 0; }
    bool IsVirtual() const { return (_flags & FunctionFlags::Virtual) != 0; }
    bool IsOverride() const { return (_flags & FunctionFlags::Override) != 0; }
    bool IsPureVirtual() const { return (_flags & FunctionFlags::PureVirtual) != 0; }
    bool IsFinal() const { return (_flags & FunctionFlags::Final) != 0; }
    bool IsStatic() const { return (_flags & FunctionFlags::Static) != 0; }
    bool IsDefault() const { return (_flags & FunctionFlags::Default) != 0; }
    bool IsDeleted() const { return (_flags & FunctionFlags::Delete) != 0; }
    bool IsInline() const { return (_flags & FunctionFlags::Inline) != 0; }

    virtual bool Visit(IASTVisitor & visitor) const = 0;

private:
    std::string _type;
    ParameterList _parameters;
    FunctionFlags _flags;
};

class Constructor : public FunctionBase
{
public:
    using Ptr = std::shared_ptr<Constructor>;
    using List = std::vector<Ptr>;
    Constructor() = delete;

    explicit Constructor(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier,
                         ParameterList parameters,
                         FunctionFlags flags)
        : FunctionBase(std::move(parent), std::move(name), accessSpecifier,
                   "", std::move(parameters), flags)
    {
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

class Destructor : public FunctionBase
{
public:
    using Ptr = std::shared_ptr<Destructor>;
    using List = std::vector<Ptr>;
    Destructor() = delete;

    explicit Destructor(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier,
                        FunctionFlags flags)
        : FunctionBase(std::move(parent), std::move(name), accessSpecifier,
                   "", ParameterList(), flags)
    {
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

class Method : public FunctionBase
{
public:
    using Ptr = std::shared_ptr<Method>;
    using List = std::vector<Ptr>;

    Method() = delete;
    explicit Method(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier,
                        std::string type, ParameterList parameters,
                        FunctionFlags flags)
        : FunctionBase(std::move(parent), std::move(name), accessSpecifier,
                   std::move(type), std::move(parameters), flags)
    {
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

class Function : public FunctionBase
{
public:
    using Ptr = std::shared_ptr<Function>;
    using List = std::vector<Ptr>;

    Function() = delete;
    explicit Function(Declaration::WeakPtr parent, std::string name,
                      std::string type, ParameterList parameters,
                      FunctionFlags flags)
        : FunctionBase(std::move(parent), std::move(name), AccessSpecifier::Invalid,
                       std::move(type), std::move(parameters), flags)
    {
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

class FunctionTemplate : public FunctionBase
{
public:
    using Ptr = std::shared_ptr<FunctionTemplate>;
    using List = std::vector<Ptr>;

    FunctionTemplate() = delete;
    explicit FunctionTemplate(Declaration::WeakPtr parent, std::string name,
                      std::string type, ParameterList parameters,
                      FunctionFlags flags)
        : FunctionBase(std::move(parent), std::move(name), AccessSpecifier::Invalid,
                       std::move(type), std::move(parameters), flags)
        , _templateParameters()
    {
    }

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
