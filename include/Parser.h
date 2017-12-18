#pragma once

#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>
#include <cstring>
#include <map>
#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Enum.h"
#include "include/Namespace.h"
#include "include/Class.h"
#include "include/Struct.h"
#include "include/ClassTemplate.h"
#include "include/AST.h"
#include "include/ASTCollection.h"
#include "include/SymbolStack.h"

namespace CPPParser
{

using TokenLookupMap = std::map<CXCursor, Declaration::Ptr>;
using TypeLookupMap = std::map<std::string, Declaration::Ptr>;

using OptionsList = std::vector<std::string>;
class Parser
{
public:
    Parser() = delete;
    explicit Parser(const std::string & path);

    bool Parse(const OptionsList & options);

    const AST & GetAST() const { return _ast; }
    const ASTCollection & GetASTCollection() const { return _astCollection; }

    void Show(std::ostream & stream);
    void TraverseTree(std::ostream & stream);

    void PrintToken(CXCursor token, CXCursor parentToken);
    void HandleToken(CXCursor token, CXCursor parentToken);

private:
    std::string _path;
    std::string _fileName;
    ASTCollection _astCollection;
    AST _ast;
    CXCursor _token;
    CXCursor _parentToken;
    SymbolStack<CXCursor> _traversalStack;
    TokenLookupMap _tokenLookupMapTraversal;
    TypeLookupMap _typeLookupMap;

    void AddToMap(Declaration::Ptr object);
    void AddNamespace(CXCursor token, CXCursor parentToken);
    void AddClass(CXCursor token, CXCursor parentToken);
    void AddStruct(CXCursor token, CXCursor parentToken);
    void AddConstructor(CXCursor token, CXCursor parentToken);
    void AddDestructor(CXCursor token, CXCursor parentToken);
    void AddMethod(CXCursor token, CXCursor parentToken);
    void AddDataMember(CXCursor token, CXCursor parentToken);
    void AddEnum(CXCursor token, CXCursor parentToken);
    void AddEnumValue(CXCursor token, CXCursor parentToken);
    void AddTypedef(CXCursor token, CXCursor parentToken);
    void AddVariable(CXCursor token, CXCursor parentToken);
    void AddFunction(CXCursor token, CXCursor parentToken);
    void AddBaseClass(CXCursor token, CXCursor parentToken);
    void AddFunctionTemplate(CXCursor token, CXCursor parentToken);
    void AddClassTemplate(CXCursor token, CXCursor parentToken);
    void AddTemplateTypeParameter(CXCursor token, CXCursor parentToken);
    void AddAccessSpecifier(CXCursor token, CXCursor parentToken);
    void AddInclude(CXCursor token, CXCursor parentToken);

    void ShowTypeMap();
};

} // namespace CPPParser
