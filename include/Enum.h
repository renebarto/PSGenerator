#pragma once

#include <vector>
#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Declaration.h"
#include "include/IASTVisitor.h"

using namespace Utility;

namespace CPPParser
{

class EnumConstant
{
public:
    EnumConstant() = delete;
    explicit EnumConstant(std::string name, long long value)
        : _name(std::move(name))
        , _value(value)
    {
    }
    explicit EnumConstant(CXCursor token)
        : _name(ConvertString(clang_getCursorSpelling(token)))
        , _value(clang_getEnumConstantDeclValue(token))
    {
    }

    const std::string & Name() const { return _name; }
    long long Value() const { return _value; }

    void Show(std::ostream & stream, int indent) const
    {
        stream << Indent(indent) << Name() << " = " << _value << std::endl;
    }
    void GenerateCode(std::ostream & stream, int indent) const
    {
        stream << Indent(indent) << Name() << " = " << _value << "," << std::endl;
    }

private:
    std::string _name;
    long long _value;
};

using EnumConstantList = std::vector<EnumConstant>;

class Enum : public Declaration
{
public:
    using Ptr = std::shared_ptr<Enum>;
    using List = std::vector<Ptr>;

    Enum() = delete;
    explicit Enum(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier,
                  std::string type)
        : Declaration(std::move(parent), std::move(name), accessSpecifier)
        , _type(std::move(type))
    {}

    const std::string & Type() const { return _type; }
    const EnumConstantList & Values() const { return _values; }

    virtual bool Visit(IASTVisitor & visitor) const override
    {
        bool ok = true;
        if (!visitor.Enter(*this))
            ok = false;
        for (auto const & value : _values)
        {
            if (!visitor.Enter(value))
                ok = false;
            if (!visitor.Leave(value))
                ok = false;
        }
        if (!visitor.Leave(*this))
            ok = false;
        return ok;
    }

    void AddValue(const std::string & name,long long value)
    {
        _values.emplace_back(name, value);
    }
    void AddValue(CXCursor token)
    {
        _values.emplace_back(token);
    }

private:
    std::string _type;
    std::vector<EnumConstant> _values;
};

} // namespace CPPParser
