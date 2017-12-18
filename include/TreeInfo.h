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
#include <include/ClassTemplate.h>
#include <include/Namespace.h>
#include <include/PreprocessorDirectives.h>

using namespace std;

namespace CPPParser
{

class TreeInfo : public IASTVisitor
{
public:
    explicit TreeInfo(std::ostream & stream)
        : _stream(stream)
        , _indent()
        , _namespaceNesting()
    {
    }

    bool ParentIsObject(const Declaration & element)
    {
        return (dynamic_pointer_cast<Object>(element.Parent()) != nullptr);
    }

    bool ParentIsNamespace(const Declaration &element)
    {
        return (dynamic_pointer_cast<Namespace>(element.Parent()) != nullptr);
    }

    void EnterGlobalNamespace()
    {
        _stream << Indent(_indent) << "Namespace <global>" << endl;
        ++_indent;
    }
    void LeaveGlobalNamespace()
    {
        --_indent;
        _stream << Indent(_indent) << "Namespace end <global>" << endl;
    }

    virtual bool Enter(const AST &) override
    {
        _namespaceNesting = 0;
        _indent = 0;
        _stream << Indent(_indent) << "AST begin" << endl;
        ++_indent;
        EnterGlobalNamespace();
        return true;
    }
    virtual bool Leave(const AST &) override
    {
        LeaveGlobalNamespace();
        --_indent;
        _stream << Indent(_indent) << "AST end" << endl;
        return true;
    }

    virtual bool Enter(const ASTCollection &) override
    {
        _namespaceNesting = 0;
        _indent = 0;
        _stream << Indent(_indent) << "ASTCollection begin" << endl;
        ++_indent;
        EnterGlobalNamespace();
        return true;
    }
    virtual bool Leave(const ASTCollection &) override
    {
        LeaveGlobalNamespace();
        --_indent;
        _stream << Indent(_indent) << "ASTCollection end" << endl;
        return true;
    }

    virtual bool Enter(const Typedef & element) override
    {
        _stream << Indent(_indent) << "Typedef " << element.Name() << ": " << element.Type() << endl;
        return true;
    }
    virtual bool Leave(const Typedef & element) override
    {
        return true;
    }

    virtual bool Enter(const EnumConstant & element) override
    {
        _stream << Indent(_indent) << "EnumConstant " << element.Name() << " = " << element.Value() << endl;
        return true;
    }
    virtual bool Leave(const EnumConstant & element) override
    {
        return true;
    }

    virtual bool Enter(const Enum & element) override
    {
        _stream << Indent(_indent) << "Enum " << (element.Name().empty() ? "<anonymous>" : element.Name());
        if (element.Type().empty())
        {
            _stream << ": <default base>";
        }
        else
        {
            _stream << ": " << element.Type();
        }
        _stream << endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const Enum & element) override
    {
        --_indent;
        _stream << Indent(_indent) << "Enum end " << (element.Name().empty() ? "<anonymous>" : element.Name()) << endl;
        return true;
    }

    bool EnterFunctionBase(const FunctionBase & element)
    {
        _stream << element.Name() << ": " << element.Type() << endl;
        _stream << Indent(_indent + 1) << "Qualifiers:";
        if (element.IsInline())
            _stream << " inline";
        if (element.IsStatic())
            _stream << " static";
        if (element.IsVirtual())
            _stream << " virtual";
        if (element.IsConst())
            _stream << " const";
        if (element.IsOverride())
            _stream << " override";
        if (element.IsFinal())
            _stream << " final";
        if (element.IsPureVirtual())
            _stream << " purevirtual";
        if (element.IsDefault())
            _stream << " default";
        if (element.IsDeleted())
            _stream << " delete";
        _stream << endl;
        _stream << Indent(_indent + 1) << "Parameters:" << endl;

        for (auto const & parameter : element.Parameters())
        {
            _stream << Indent(_indent + 2) << parameter.Name() << ": " << parameter.Type() << endl;
        }
        return true;
    }
    bool LeaveFunctionBase(const FunctionBase & element)
    {
        _stream << element.Name() << endl;
        return true;
    }

    virtual bool Enter(const Constructor & element) override
    {
        _stream << Indent(_indent) << "Constructor ";
        EnterFunctionBase(element);
        ++_indent;
        return true;
    }
    virtual bool Leave(const Constructor & element) override
    {
        --_indent;
        _stream << Indent(_indent) << "Constructor end ";
        LeaveFunctionBase(element);
        return true;
    }

    virtual bool Enter(const Destructor & element) override
    {
        _stream << Indent(_indent) << "Destructor ";
        EnterFunctionBase(element);
        ++_indent;
        return true;
    }
    virtual bool Leave(const Destructor & element) override
    {
        --_indent;
        _stream << Indent(_indent) << "Destructor end ";
        LeaveFunctionBase(element);
        return true;
    }

    virtual bool Enter(const Method & element) override
    {
        _stream << Indent(_indent) << "Method ";
        EnterFunctionBase(element);
        ++_indent;
        return true;
    }
    virtual bool Leave(const Method & element) override
    {
        --_indent;
        _stream << Indent(_indent) << "Method end ";
        LeaveFunctionBase(element);
        return true;
    }

    virtual bool Enter(const Function & element) override
    {
        _stream << Indent(_indent) << "Function ";
        EnterFunctionBase(element);
        ++_indent;
        return true;
    }
    virtual bool Leave(const Function & element) override
    {
        --_indent;
        _stream << Indent(_indent) << "Function end ";
        LeaveFunctionBase(element);
        return true;
    }

    virtual bool Enter(const FunctionTemplate & element) override
    {
        _stream << Indent(_indent) << "FunctionTemplate ";
        _stream << element.Name() << ": " << element.Type() << endl;
        _stream << Indent(_indent + 1) << "Template parameters:" << endl;
        for (auto const & parameter : element.TemplateParameters())
        {
            _stream << Indent(_indent + 2) << parameter << endl;
        }
        _stream << Indent(_indent + 1) << "Qualifiers:";
        if (element.IsInline())
            _stream << " inline";
        if (element.IsStatic())
            _stream << " static";
        if (element.IsVirtual())
            _stream << " virtual";
        if (element.IsConst())
            _stream << " const";
        if (element.IsOverride())
            _stream << " override";
        if (element.IsFinal())
            _stream << " final";
        if (element.IsPureVirtual())
            _stream << " purevirtual";
        if (element.IsDefault())
            _stream << " default";
        if (element.IsDeleted())
            _stream << " delete";
        _stream << endl;
        _stream << Indent(_indent + 1) << "Parameters:" << endl;
        for (auto const & parameter : element.Parameters())
        {
            _stream << Indent(_indent + 2) << parameter.Name() << ": " << parameter.Type() << endl;
        }
        ++_indent;
        return true;
    }
    virtual bool Leave(const FunctionTemplate & element) override
    {
        --_indent;
        _stream << Indent(_indent) << "FunctionTemplate end ";
        _stream << element.Name() << endl;
        return true;
    }

    virtual bool Enter(const Variable & element) override
    {
        _stream << Indent(_indent) << "Variable " << element.Name() << ": " << element.Type() << endl;
        return true;
    }
    virtual bool Leave(const Variable & element) override
    {
        return true;
    }

    virtual bool Enter(const DataMember & element) override
    {
        _stream << Indent(_indent) << "Member variable " << element.Name() << ": " << element.Type() << endl;
        return true;
    }
    virtual bool Leave(const DataMember & element) override
    {
        return true;
    }

    void ObjectInheritance(const Object & element)
    {
        if (!element.BaseTypes().empty())
        {
            _stream << Indent(_indent + 1) << "Inheritance: " << endl;
            for (auto const & inheritance : element.BaseTypes())
            {
                _stream << Indent(_indent + 2) << inheritance->Name() << ": " << inheritance->Access();
                if (inheritance->IsVirtual())
                    _stream << " virtual";
                _stream << endl;
            }
        }
    }

    virtual bool Enter(const Class & element) override
    {
        _stream << Indent(_indent) << "Class " << element.Name() << endl;
        ObjectInheritance(element);
        ++_indent;
        return true;
    }
    virtual bool Leave(const Class & element) override
    {
        --_indent;
        _stream << Indent(_indent) << "Class end " << element.Name() << endl;
        return true;
    }

    virtual bool Enter(const Struct & element) override
    {
        _stream << Indent(_indent) << "Struct " << element.Name() << endl;
        ObjectInheritance(element);
        ++_indent;
        return true;
    }
    virtual bool Leave(const Struct & element) override
    {
        --_indent;
        _stream << Indent(_indent) << "Struct end " << element.Name() << endl;
        return true;
    }

    virtual bool Enter(const ClassTemplate & element) override
    {
        _stream << Indent(_indent) << "ClassTemplate " << element.Name() << endl;
        ObjectInheritance(element);
        _stream << Indent(_indent + 1) << "Template parameters:" << endl;
        for (auto const & parameter : element.TemplateParameters())
        {
            _stream << Indent(_indent + 2) << parameter << endl;
        }
        ++_indent;
        return true;
    }
    virtual bool Leave(const ClassTemplate & element) override
    {
        --_indent;
        _stream << Indent(_indent) << "ClassTemplate end " << element.Name() << endl;
        return true;
    }

    virtual bool Enter(const Namespace & element) override
    {
        if (_namespaceNesting == 0)
            LeaveGlobalNamespace();
        ++_namespaceNesting;
        _stream << Indent(_indent) << "Namespace " << (element.Name().empty() ? "<anonymous>" : element.Name()) << endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const Namespace & element) override
    {
        --_namespaceNesting;
        --_indent;
        _stream << Indent(_indent) << "Namespace end " << (element.Name().empty() ? "<anonymous>" : element.Name()) << endl;
        if (_namespaceNesting == 0)
            EnterGlobalNamespace();
        return true;
    }

    virtual bool Enter(const IncludeDirective & element) override
    {
        _stream << Indent(_indent) << "Include "
                << (element.IncludeType() == IncludeSpecifier::Local ? '"' : '<')
                << element.Name()
                << (element.IncludeType() == IncludeSpecifier::Local ? '"' : '>')
                << endl;
        return true;
    }
    virtual bool Leave(const IncludeDirective & element) override
    {
        return true;
    }

    virtual bool Enter(const IfdefDirective & element) override
    {
        _stream << Indent(_indent) << "Ifdef "
                << element.Name()
                << endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const IfdefDirective & element) override
    {
        --_indent;
        _stream << Indent(_indent) << "Endif"
                << endl;
        return true;
    }

    virtual bool Enter(const IfDirective & element) override
    {
        _stream << Indent(_indent) << "If "
                << element.Name()
                << endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const IfDirective & element) override
    {
        --_indent;
        _stream << Indent(_indent) << "Endif"
                << endl;
        return true;
    }

    virtual bool Enter(const DefineDirective & element) override
    {
        _stream << Indent(_indent) << "Define "
                << element.Name()
                << endl;
        return true;
    }
    virtual bool Leave(const DefineDirective & element) override
    {
        return true;
    }

    virtual bool Enter(const UndefDirective & element) override
    {
        _stream << Indent(_indent) << "Undef "
                << element.Name()
                << endl;
        return true;
    }
    virtual bool Leave(const UndefDirective & element) override
    {
        return true;
    }

public:
    std::ostream & _stream;
    int _indent;
    int _namespaceNesting;
};

} // namespace CPPParser
