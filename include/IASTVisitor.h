#pragma once

namespace CPPParser
{

class AST;
class Typedef;
class EnumConstant;
class Enum;
class Parameter;
class Constructor;
class Destructor;
class Method;
class Function;
class Variable;
class DataMember;
class Inheritance;
class Class;
class Struct;
class Namespace;

class IASTVisitor
{
public:
    virtual ~IASTVisitor() = default;

    virtual bool Enter(const AST &) = 0;
    virtual bool Leave(const AST &) = 0;

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

    virtual bool Enter(const Variable &) = 0;
    virtual bool Leave(const Variable &) = 0;

    virtual bool Enter(const DataMember &) = 0;
    virtual bool Leave(const DataMember &) = 0;

    virtual bool Enter(const Class &) = 0;
    virtual bool Leave(const Class &) = 0;

    virtual bool Enter(const Struct &) = 0;
    virtual bool Leave(const Struct &) = 0;

    virtual bool Enter(const Namespace &) = 0;
    virtual bool Leave(const Namespace &) = 0;

};

} // namespace CPPParser