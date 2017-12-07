#pragma once

#include <include/IASTVisitor.h>
#include <include/AST.h>
#include <include/Typedef.h>
#include <include/Inheritance.h>
#include <include/Enum.h>
#include <include/Function.h>
#include <include/Variable.h>
#include <include/Class.h>
#include <include/Struct.h>
#include <include/Namespace.h>

using namespace std;
//using namespace Utility;

namespace CPPParser
{

inline void TRACE(const std::string & function, const std::string & text)
{
    cerr << function << "(" << text << ")" << endl;
}

inline void TRACE2(const std::string & function, const std::string & text, const std::string & name)
{
    cerr << function << "(" << text << ") : " << name << endl;
}

class CodeGenerator : public IASTVisitor
{
public:
    CodeGenerator(std::ostream & stream)
        : _stream(stream)
        , _indent()
    {
    }

    virtual bool Enter(const AST &) override
    {
        TRACE(__func__, "AST");
        _indent = 0;
        return true;
    }
    virtual bool Leave(const AST &) override
    {
        TRACE(__func__, "AST");
        return true;
    }

    virtual bool Enter(const Typedef & element) override
    {
        TRACE2(__func__, "Typedef", element.Name());
        return true;
    }
    virtual bool Leave(const Typedef & element) override
    {
        TRACE2(__func__, "Typedef", element.Name());
        return true;
    }

    virtual bool Enter(const Inheritance & element) override
    {
        TRACE2(__func__, "Inheritance", element.Name());
        return true;
    }
    virtual bool Leave(const Inheritance & element) override
    {
        TRACE2(__func__, "Inheritance", element.Name());
        return true;
    }

    virtual bool Enter(const EnumConstant & element) override
    {
        TRACE2(__func__, "EnumConstant", element.Name());
        _stream << Indent(_indent) << element.Name() << " = " << element.Value() << "," << std::endl;
        return true;
    }
    virtual bool Leave(const EnumConstant & element) override
    {
        TRACE2(__func__, "EnumConstant", element.Name());
        return true;
    }

    virtual bool Enter(const Enum & element) override
    {
        TRACE2(__func__, "Enum", element.Name());
        _stream << Indent(_indent) << "enum "
                << (element.Name().empty() ? "" : element.Name() + " ");
        if (!element.Type().empty())
        {
            _stream << ": " << element.Type() << " ";
        }
        _stream << "{" << std::endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const Enum & element) override
    {
        TRACE2(__func__, "Enum", element.Name());
        --_indent;
        _stream << Indent(_indent) << "}; // enum "
                << (element.Name().empty() ? "<anonymous>" : element.Name()) << std::endl;
        return true;
    }

    virtual bool Enter(const Parameter & element) override
    {
        TRACE2(__func__, "Parameter", element.Name());
        return true;
    }
    virtual bool Leave(const Parameter & element) override
    {
        TRACE2(__func__, "Parameter", element.Name());
        return true;
    }

    virtual bool Enter(const Constructor & element) override
    {
        TRACE2(__func__, "Constructor", element.Name());
        return true;
    }
    virtual bool Leave(const Constructor & element) override
    {
        TRACE2(__func__, "Constructor", element.Name());
        return true;
    }

    virtual bool Enter(const Destructor & element) override
    {
        TRACE2(__func__, "Destructor", element.Name());
        return true;
    }
    virtual bool Leave(const Destructor & element) override
    {
        TRACE2(__func__, "Destructor", element.Name());
        return true;
    }

    virtual bool Enter(const Method & element) override
    {
        TRACE2(__func__, "Method", element.Name());
        return true;
    }
    virtual bool Leave(const Method & element) override
    {
        TRACE2(__func__, "Method", element.Name());
        return true;
    }

    virtual bool Enter(const Function & element) override
    {
        TRACE2(__func__, "Function", element.Name());
        _stream << Indent(_indent);
        if (element.IsInline())
            _stream << "inline ";
        if (element.IsStatic())
            _stream << "static ";
        if (element.IsVirtual())
            _stream << "virtual ";
        _stream << element.Type() << " " << element.Name() << "(";
        bool firstParameter = true;
        for (auto const & parameter : element.Parameters())
        {
            if (!firstParameter)
            {
                _stream << ", ";
            }
            parameter.GenerateCode(_stream, _indent + 1);
            firstParameter = false;
        }
        _stream << ")";
        if (element.IsConst())
            _stream << " const";
        if (element.IsOverride())
            _stream << " override";
        if (element.IsFinal())
            _stream << " override";
        if (element.IsPureVirtual())
            _stream << " = 0";
        _stream << ";" << std::endl;
        return true;
    }
    virtual bool Leave(const Function & element) override
    {
        TRACE2(__func__, "Function", element.Name());
        return true;
    }

    virtual bool Enter(const Variable & element) override
    {
        TRACE2(__func__, "Variable", element.Name());
        _stream << Indent(_indent) << element.Type() << " " << element.Name() << ";" << endl;

        return true;
    }
    virtual bool Leave(const Variable & element) override
    {
        TRACE2(__func__, "Variable", element.Name());
        return true;
    }

    virtual bool Enter(const DataMember & element) override
    {
        TRACE2(__func__, "DataMember", element.Name());
        return true;
    }
    virtual bool Leave(const DataMember & element) override
    {
        TRACE2(__func__, "DataMember", element.Name());
        return true;
    }

    virtual bool Enter(const Class & element) override
    {
        TRACE2(__func__, "Class", element.Name());
        _stream << Indent(_indent) << "class " << element.Name();
        auto baseTypes = element.BaseTypes();
        if (!baseTypes.empty())
        {
            bool firstBase = true;
            for (size_t index = 0; index < baseTypes.size(); ++index)
            {
                if (firstBase)
                    _stream << " : ";
                else
                    _stream << ", ";

//                baseTypes[index]->GenerateCode(_stream, _indent);
                firstBase = false;
            }
        }
        _stream << " {" << std::endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const Class & element) override
    {
        TRACE2(__func__, "Class", element.Name());
        --_indent;
        _stream << Indent(_indent) << "}; // class " << element.Name() << std::endl;
        return true;
    }

    virtual bool Enter(const Struct & element) override
    {
        TRACE2(__func__, "Struct", element.Name());
        _stream << Indent(_indent) << "struct " << element.Name();
        auto baseTypes = element.BaseTypes();
        if (!baseTypes.empty())
        {
            bool firstBase = true;
            for (size_t index = 0; index < baseTypes.size(); ++index)
            {
                if (firstBase)
                    _stream << " : ";
                else
                    _stream << ", ";

//                baseTypes[index]->GenerateCode(_stream, _indent);
                firstBase = false;
            }
        }
        _stream << " {" << std::endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const Struct & element) override
    {
        TRACE2(__func__, "Struct", element.Name());
        --_indent;
        _stream << Indent(_indent) << "}; // struct " << element.Name() << std::endl;
        return true;
    }

    virtual bool Enter(const Namespace & element) override
    {
        TRACE2(__func__, "Namespace", element.Name());
        _stream << Indent(_indent) << "namespace " << (element.Name().empty() ? "" : element.Name() + " ") << "{" << endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const Namespace & element) override
    {
        TRACE2(__func__, "Namespace", element.Name());
        --_indent;
        _stream << Indent(_indent) << "} // namespace " << (element.Name().empty() ? "<anonymous>" : element.Name()) << endl;
        return true;
    }

public:
    std::ostream & _stream;
    int _indent;
};

} // namespace CPPParser
