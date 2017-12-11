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
    void Show(std::ostream & stream, int indent) const
    {
        stream << Indent(indent) << _name << ": " << _type << std::endl;
    }
    void GenerateCode(std::ostream & stream, int indent) const
    {
        stream << _type << " " << _name;
    }

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
    void Show(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << Name() << ": " << _type << std::endl;
        ShowContents(stream, indent);
    }
    void GenerateCode(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent);
        if (IsStatic())
            stream << "static ";
        if (IsVirtual())
            stream << "virtual ";
        stream << Type() << " " << Name() << "(";
        GenerateCodeContents(stream, indent);
        stream << ")";
        if (IsConst())
            stream << " const";
        if (IsOverride())
            stream << " override";
        if (IsFinal())
            stream << " override";
        if (IsPureVirtual())
            stream << " = 0";
        stream << ";" << std::endl;
    }
    virtual void ShowContents(std::ostream & stream, int indent) const
    {
        for (auto const & parameter : _parameters)
            parameter.Show(stream, indent + 1);
    }
    virtual void GenerateCodeContents(std::ostream & stream, int indent) const
    {
        bool firstParameter = true;
        for (auto const & parameter : _parameters)
        {
            if (!firstParameter)
            {
                stream << ", ";
            }
            parameter.GenerateCode(stream, indent + 1);
            firstParameter = false;
        }
    }

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

    virtual bool Visit(IASTVisitor & visitor) const override
    {
        bool ok = true;
        if (!visitor.Enter(*this))
            ok = false;
        if (!visitor.Leave(*this))
            ok = false;
        return ok;
    }
    void Show(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << "Constructor " << Name() << ": " << Type() << std::endl;
        ShowContents(stream, indent);
    }
    void GenerateCode(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent);
        stream << Name() << "(";
        GenerateCodeContents(stream, indent);
        stream << ");" << std::endl;
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

    virtual bool Visit(IASTVisitor & visitor) const override
    {
        bool ok = true;
        if (!visitor.Enter(*this))
            ok = false;
        if (!visitor.Leave(*this))
            ok = false;
        return ok;
    }
    void Show(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << "Destructor " << Name() << ": " << Type() << std::endl;
        ShowContents(stream, indent);
    }
    void GenerateCode(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent);
        if (IsVirtual())
            stream << "virtual ";
        stream << Name() << "();" << std::endl;
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

    virtual bool Visit(IASTVisitor & visitor) const override
    {
        bool ok = true;
        if (!visitor.Enter(*this))
            ok = false;
        if (!visitor.Leave(*this))
            ok = false;
        return ok;
    }
    void Show(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << "Method " << Name() << ": " << Type() << std::endl;
        ShowContents(stream, indent);
    }
    void GenerateCode(std::ostream & stream, int indent) const override
    {
        FunctionBase::GenerateCode(stream, indent);
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

    virtual bool Visit(IASTVisitor & visitor) const override
    {
        bool ok = true;
        if (!visitor.Enter(*this))
            ok = false;
        if (!visitor.Leave(*this))
            ok = false;
        return ok;
    }
    void Show(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << "Method " << Name() << ": " << Type() << std::endl;
        ShowContents(stream, indent);
    }
    void GenerateCode(std::ostream & stream, int indent) const override
    {
        Function::GenerateCode(stream, indent);
    }
};

} // namespace CPPParser
