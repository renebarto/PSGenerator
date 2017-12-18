#pragma once

namespace CPPParser
{

class AST;
class ASTCollection;
// Type declarations
class Typedef;
class EnumConstant;
class Enum;
class Parameter;
class Constructor;
class Destructor;
class Method;
class Function;
class FunctionTemplate;
class Variable;
class DataMember;
class Inheritance;
class Class;
class Struct;
class ClassTemplate;
class Namespace;
// Preprocessor directives
class IncludeDirective;
class IfdefDirective;
class IfDirective;
class DefineDirective;
class UndefDirective;

// Statements

class IASTVisitor
{
public:
    virtual ~IASTVisitor() = default;

    virtual bool Enter(const AST &) = 0;
    virtual bool Leave(const AST &) = 0;

    virtual bool Enter(const ASTCollection &) = 0;
    virtual bool Leave(const ASTCollection &) = 0;

    // Type declarations
    virtual bool Enter(const Typedef &) = 0;
    virtual bool Leave(const Typedef &) = 0;

    virtual bool Enter(const EnumConstant &) = 0;
    virtual bool Leave(const EnumConstant &) = 0;

    virtual bool Enter(const Enum &) = 0;
    virtual bool Leave(const Enum &) = 0;

    virtual bool Enter(const Constructor &) = 0;
    virtual bool Leave(const Constructor &) = 0;

    virtual bool Enter(const Destructor &) = 0;
    virtual bool Leave(const Destructor &) = 0;

    virtual bool Enter(const Method &) = 0;
    virtual bool Leave(const Method &) = 0;

    virtual bool Enter(const Function &) = 0;
    virtual bool Leave(const Function &) = 0;

    virtual bool Enter(const FunctionTemplate &) = 0;
    virtual bool Leave(const FunctionTemplate &) = 0;

    virtual bool Enter(const Variable &) = 0;
    virtual bool Leave(const Variable &) = 0;

    virtual bool Enter(const DataMember &) = 0;
    virtual bool Leave(const DataMember &) = 0;

    virtual bool Enter(const Class &) = 0;
    virtual bool Leave(const Class &) = 0;

    virtual bool Enter(const Struct &) = 0;
    virtual bool Leave(const Struct &) = 0;

    virtual bool Enter(const ClassTemplate &) = 0;
    virtual bool Leave(const ClassTemplate &) = 0;

    virtual bool Enter(const Namespace &) = 0;
    virtual bool Leave(const Namespace &) = 0;

    // Preprocessor
    virtual bool Enter(const IncludeDirective &) = 0;
    virtual bool Leave(const IncludeDirective &) = 0;

    virtual bool Enter(const IfdefDirective &) = 0;
    virtual bool Leave(const IfdefDirective &) = 0;

    virtual bool Enter(const IfDirective &) = 0;
    virtual bool Leave(const IfDirective &) = 0;

    virtual bool Enter(const DefineDirective &) = 0;
    virtual bool Leave(const DefineDirective &) = 0;

    virtual bool Enter(const UndefDirective &) = 0;
    virtual bool Leave(const UndefDirective &) = 0;

    // Statements

};

} // namespace CPPParser