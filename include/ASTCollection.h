#pragma once

#include <map>
#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Container.h"
#include "include/IASTVisitor.h"
#include "include/Namespace.h"
#include "include/Class.h"
#include "include/Struct.h"
#include "include/ClassTemplate.h"
#include "include/Enum.h"
#include "include/SymbolStack.h"

using namespace Utility;

namespace CPPParser
{

using TokenLookupMap = std::map<CXCursor, Declaration::Ptr>;

class ASTCollection : public Container
{
public:
    ASTCollection();
    virtual bool IsValid() const override { return false; }
    void Show(std::ostream & stream, int indent) const;
    void GenerateCode(std::ostream & stream, int indent) const;

    virtual std::string QualifiedDescription() const override { return ""; }
    virtual std::string QualifiedName() const override { return ""; }
    virtual bool TraverseBegin(IASTVisitor & visitor) const override;
    virtual bool TraverseEnd(IASTVisitor & visitor) const override;
    virtual bool Visit(IASTVisitor & visitor) const override;

    Declaration::Ptr Find(CXCursor token) const;
    bool FindNamespaceByName(Declaration::Ptr parent, const std::string & name, Namespace::Ptr & result);
    bool FindClassByName(Declaration::Ptr parent, const std::string & name, Class::Ptr & result);
    bool FindStructByName(Declaration::Ptr parent, const std::string & name, Struct::Ptr & result);
    bool FindClassTemplateByName(Declaration::Ptr parent, const std::string & name, ClassTemplate::Ptr & result);
    bool FindEnumByName(Declaration::Ptr parent, const std::string & name, Enum::Ptr & result);

    void AddToMap(CXCursor token, Declaration::Ptr object);
    Declaration::Ptr AddNamespace(CXCursor token, CXCursor parentToken);
    Declaration::Ptr AddClass(CXCursor token, CXCursor parentToken);
    Declaration::Ptr AddStruct(CXCursor token, CXCursor parentToken);
    Declaration::Ptr AddConstructor(CXCursor token, CXCursor parentToken);
    Declaration::Ptr AddDestructor(CXCursor token, CXCursor parentToken);
    Declaration::Ptr AddMethod(CXCursor token, CXCursor parentToken);
    Declaration::Ptr AddDataMember(CXCursor token, CXCursor parentToken);
    Declaration::Ptr AddEnum(CXCursor token, CXCursor parentToken);
    void AddEnumValue(CXCursor token, CXCursor parentToken);
    Declaration::Ptr AddTypedef(CXCursor token, CXCursor parentToken);
    Declaration::Ptr AddVariable(CXCursor token, CXCursor parentToken);
    Declaration::Ptr AddFunction(CXCursor token, CXCursor parentToken);
    void AddBaseClass(CXCursor token, CXCursor parentToken, Declaration::Ptr baseType);
    Declaration::Ptr AddFunctionTemplate(CXCursor token, CXCursor parentToken);
    Declaration::Ptr AddClassTemplate(CXCursor token, CXCursor parentToken);
    void AddTemplateTypeParameter(CXCursor token, CXCursor parentToken);

    void ShowInfo();

private:
    SymbolStack<CXCursor> _stack;
    TokenLookupMap _tokenLookupMap;

    void UpdateStack(CXCursor token, CXCursor parentToken);
};

} // namespace CPPParser
