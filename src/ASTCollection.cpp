#include "include/ASTCollection.h"

#include <fstream>
#include <iomanip>
#include "include/Utility.h"
#include "include/Container.h"
#include <include/TreeInfo.h>
#include <clang-c/Index.h>
#include "include/Namespace.h"
#include "include/CodeGenerator.h"

using namespace std;
using namespace Utility;

namespace CPPParser
{

ASTCollection::ASTCollection()
    : Container(WeakPtr(), SourceLocation(), "ASTCollection", AccessSpecifier::Invalid)
    , _stack()
    , _tokenLookupMap()
{
}

bool ASTCollection::TraverseBegin(IASTVisitor & visitor) const
{
    return visitor.Enter(*this);
}

bool ASTCollection::TraverseEnd(IASTVisitor & visitor) const
{
    return visitor.Leave(*this);
}

bool ASTCollection::Visit(IASTVisitor & visitor) const
{
    bool ok = true;
    if (!visitor.Enter(*this))
        ok = false;
    if (!VisitChildren(visitor))
        ok = false;
    if (!visitor.Leave(*this))
        ok = false;
    return ok;
}

void ASTCollection::Show(std::ostream & stream, int indent) const
{
    TreeInfo treeInfo(stream);
    Visit(treeInfo);
}

void ASTCollection::GenerateCode(std::ostream & stream, int indent) const
{
    CodeGenerator codeGenerator(stream);
    Visit(codeGenerator);
}

Declaration::Ptr ASTCollection::Find(CXCursor token) const
{
    auto it = _tokenLookupMap.find(token);
    Declaration::Ptr object = nullptr;
    if (it != _tokenLookupMap.end())
        object = it->second;
    return object;
}

bool ASTCollection::FindNamespaceByName(Declaration::Ptr parent, const std::string & name, Namespace::Ptr & result)
{
    result = {};
    if (parent == nullptr)
        return FindNamespace(name, result);
    auto parentContainer = dynamic_pointer_cast<Container>(parent);
    if (parentContainer != nullptr)
        return parentContainer->FindNamespace(name, result);
    return false;
}

bool ASTCollection::FindClassByName(Declaration::Ptr parent, const std::string & name, Class::Ptr & result)
{
    result = {};
    if (parent == nullptr)
        return FindClass(name, result);
    auto parentContainer = dynamic_pointer_cast<Container>(parent);
    if (parentContainer != nullptr)
        return parentContainer->FindClass(name, result);
    return false;
}

bool ASTCollection::FindStructByName(Declaration::Ptr parent, const std::string & name, Struct::Ptr & result)
{
    result = {};
    if (parent == nullptr)
        return FindStruct(name, result);
    auto parentContainer = dynamic_pointer_cast<Container>(parent);
    if (parentContainer != nullptr)
        return parentContainer->FindStruct(name, result);
    return false;
}

bool ASTCollection::FindClassTemplateByName(Declaration::Ptr parent, const std::string & name, ClassTemplate::Ptr & result)
{
    result = {};
    if (parent == nullptr)
        return FindClassTemplate(name, result);
    auto parentContainer = dynamic_pointer_cast<Container>(parent);
    if (parentContainer != nullptr)
        return parentContainer->FindClassTemplate(name, result);
    return false;
}

bool ASTCollection::FindEnumByName(Declaration::Ptr parent, const std::string & name, Enum::Ptr & result)
{
    result = {};
    if (parent == nullptr)
        return FindEnum(name, result);
    auto parentContainer = dynamic_pointer_cast<Container>(parent);
    if (parentContainer != nullptr)
        return parentContainer->FindEnum(name, result);
    return false;
}

void ASTCollection::AddToMap(CXCursor token, Declaration::Ptr object)
{
    _tokenLookupMap.insert({token, object});
}

Declaration::Ptr ASTCollection::AddNamespace(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    Namespace::Ptr object;
    std::string name = ConvertString(clang_getCursorSpelling(token));
    bool addNewObject = true;
    if (FindNamespaceByName(parent, name, object))
    {
        // If it already exists, we have a duplicate with a new token, and the same object with the new token
        cout << "Namespace already exists." << endl;
        addNewObject = false;
    }
    else
    {
        object = make_shared<Namespace>(parent, SourceLocation(token), name);
    }
    UpdateStack(token, parentToken);
    Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
    if (addNewObject)
    {
        if (parentContainer != nullptr)
        {
            parentContainer->Add(object);
        }
        else
        {
            Add(object);
        }
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr ASTCollection::AddClass(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    Class::Ptr object;
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    bool addNewObject = true;
    if (FindClassByName(parent, name, object))
    {
        // If it already exists, we have a duplicate with a new token, and the same object with the new token
        cout << "Class already exists." << endl;
        addNewObject = false;
    }
    else
    {
        object = make_shared<Class>(parent, SourceLocation(token), name, accessSpecifier);
    }
    UpdateStack(token, parentToken);
    if (addNewObject)
    {
        Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
        if (parentContainer != nullptr)
        {
            parentContainer->Add(object);
        } else
        {
            Add(object);
        }
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr ASTCollection::AddStruct(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    Struct::Ptr object;
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    bool addNewObject = true;
    if (FindStructByName(parent, name, object))
    {
        // If it already exists, we have a duplicate with a new token, and the same object with the new token
        cout << "Struct already exists." << endl;
        addNewObject = false;
    }
    else
    {
        object = make_shared<Struct>(parent, SourceLocation(token), name, accessSpecifier);
    }
    UpdateStack(token, parentToken);
    if (addNewObject)
    {
        Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
        if (parentContainer != nullptr)
        {
            parentContainer->Add(object);
        } else
        {
            Add(object);
        }
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr ASTCollection::AddConstructor(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    CXType functionType = clang_getCursorType(token);

    int numArguments = clang_Cursor_getNumArguments(token);
    ParameterList parameters;
    for (int i = 0; i < numArguments; ++i)
    {
        CXCursor parameterToken = clang_Cursor_getArgument(token, i);
        CXString parameterNameStr = clang_getCursorSpelling(parameterToken);
        CXType parameterTypeDecl = clang_getArgType(functionType, i);
        CXString parameterTypeStr = clang_getTypeSpelling(parameterTypeDecl);
        std::string parameterName = ConvertString(parameterNameStr);
        std::string parameterType = ConvertString(parameterTypeStr);

        parameters.emplace_back(parameterName, parameterType);
    }
    FunctionFlags flags {};
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<Constructor>(parent, SourceLocation(token), name, accessSpecifier, parameters, flags);
    UpdateStack(token, parentToken);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        cerr << "Parent is not an object" << endl;
        return nullptr;
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr ASTCollection::AddDestructor(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    CXType functionType = clang_getCursorType(token);
    CXType resultType = clang_getResultType(functionType);
    CXString resultTypeStr = clang_getTypeSpelling(resultType);

    FunctionFlags flags {};
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<Destructor>(parent, SourceLocation(token), name, accessSpecifier, flags);
    UpdateStack(token, parentToken);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        cerr << "Parent is not an object" << endl;
        return nullptr;
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr ASTCollection::AddMethod(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    CXType functionType = clang_getCursorType(token);
    CXType resultType = clang_getResultType(functionType);
    CXString resultTypeStr = clang_getTypeSpelling(resultType);
    std::string type = ConvertString(resultTypeStr);

    int numArguments = clang_Cursor_getNumArguments(token);
    ParameterList parameters;
    for (int i = 0; i < numArguments; ++i)
    {
        CXCursor parameterToken = clang_Cursor_getArgument(token, i);
        CXString parameterNameStr = clang_getCursorSpelling(parameterToken);
        CXType parameterTypeDecl = clang_getArgType(functionType, i);
        CXString parameterTypeStr = clang_getTypeSpelling(parameterTypeDecl);
        std::string parameterName = ConvertString(parameterNameStr);
        std::string parameterType = ConvertString(parameterTypeStr);

        parameters.emplace_back(parameterName, parameterType);
    }
    FunctionFlags flags {};
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<Method>(parent, SourceLocation(token), name, accessSpecifier, type, parameters, flags);
    UpdateStack(token, parentToken);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        // Regular function in global namespace
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr ASTCollection::AddDataMember(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    std::string type = ConvertString(clang_getTypeSpelling(clang_getCursorType(token)));

    auto object = make_shared<DataMember>(parent, SourceLocation(token), name, accessSpecifier, type);
    UpdateStack(token, parentToken);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        // Regular type in global namespace
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr ASTCollection::AddEnum(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    std::string underlyingType;
    CXType type = clang_getEnumDeclIntegerType(token);
    if (type.kind != CXType_UInt)
    {
        CXString strType = clang_getTypeSpelling(type);
        underlyingType = ConvertString(strType);
    }

    auto object = make_shared<Enum>(parent, SourceLocation(token), name, accessSpecifier, underlyingType);
    UpdateStack(token, parentToken);
    Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
    if (parentContainer != nullptr)
    {
        parentContainer->Add(object);
    }
    else
    {
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

void ASTCollection::AddEnumValue(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    Enum::Ptr parentEnum = dynamic_pointer_cast<Enum>(parent);
    if (parentEnum != nullptr)
    {
        parentEnum->AddValue(token);
    }
    else
    {
        cerr << "Parent is not an enum?" << endl;
        return;
    }
}

Declaration::Ptr ASTCollection::AddTypedef(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    std::string type = ConvertString(clang_getTypeSpelling(clang_getTypedefDeclUnderlyingType(token)));

    auto object = make_shared<Typedef>(parent, SourceLocation(token), name, accessSpecifier, type);
    UpdateStack(token, parentToken);
    Namespace::Ptr parentNamespace = dynamic_pointer_cast<Namespace>(object->Parent());
    if (parentNamespace != nullptr)
    {
        parentNamespace->Add(object);
    }
    else
    {
        // Regular type in global namespace
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr ASTCollection::AddVariable(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    std::string type = ConvertString(clang_getTypeSpelling(clang_getCursorType(token)));

    auto object = make_shared<Variable>(parent, SourceLocation(token), name, accessSpecifier, type);
    UpdateStack(token, parentToken);
    Namespace::Ptr parentNamespace = dynamic_pointer_cast<Namespace>(object->Parent());
    if (parentNamespace != nullptr)
    {
        parentNamespace->Add(object);
    }
    else
    {
        // Regular type in global namespace
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr ASTCollection::AddFunction(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    CXType functionType = clang_getCursorType(token);
    CXType resultType = clang_getResultType(functionType);
    CXString resultTypeStr = clang_getTypeSpelling(resultType);
    std::string type = ConvertString(resultTypeStr);

    int numArguments = clang_Cursor_getNumArguments(token);
    ParameterList parameters;
    for (int i = 0; i < numArguments; ++i)
    {
        CXCursor parameterToken = clang_Cursor_getArgument(token, i);
        CXString parameterNameStr = clang_getCursorSpelling(parameterToken);
        CXType parameterTypeDecl = clang_getArgType(functionType, i);
        CXString parameterTypeStr = clang_getTypeSpelling(parameterTypeDecl);
        std::string parameterName = ConvertString(parameterNameStr);
        std::string parameterType = ConvertString(parameterTypeStr);

        parameters.emplace_back(parameterName, parameterType);
    }
    FunctionFlags flags {};
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<Function>(parent, SourceLocation(token), name, type, parameters, flags);
    UpdateStack(token, parentToken);
    Namespace::Ptr parentNamespace = dynamic_pointer_cast<Namespace>(object->Parent());
    if (parentNamespace != nullptr)
    {
        parentNamespace->Add(object);
    }
    else
    {
        // Regular type in global namespace
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

void ASTCollection::AddBaseClass(CXCursor token, CXCursor parentToken, Declaration::Ptr baseType)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    bool isVirtual = (clang_isVirtualBase(token) != 0);

    CXType typeDecl = clang_getCursorType(token);
    CXType baseTypeDecl = clang_getCanonicalType(typeDecl);

    auto inheritance = make_shared<Inheritance>(parent, SourceLocation(token), name, accessSpecifier, baseType, isVirtual);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(parent);
    if (parentObject != nullptr)
    {
        parentObject->AddBase(inheritance);
    }
    else
    {
        if (parent == nullptr)
        {
            cerr << "Type is null. Type not supported yet?" << endl;
        }
        else
        {
            cerr << "Type is not an object" <<  parent->Name() << endl;
        }
        return;
    }
}

Declaration::Ptr ASTCollection::AddFunctionTemplate(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    CXType functionType = clang_getCursorType(token);
    CXType resultType = clang_getResultType(functionType);
    CXString resultTypeStr = clang_getTypeSpelling(resultType);
    std::string type = ConvertString(resultTypeStr);

    int numArguments = clang_Cursor_getNumArguments(token);
    ParameterList parameters;
    for (int i = 0; i < numArguments; ++i)
    {
        CXCursor parameterToken = clang_Cursor_getArgument(token, i);
        CXString parameterNameStr = clang_getCursorSpelling(parameterToken);
        CXType parameterTypeDecl = clang_getArgType(functionType, i);
        CXString parameterTypeStr = clang_getTypeSpelling(parameterTypeDecl);
        std::string parameterName = ConvertString(parameterNameStr);
        std::string parameterType = ConvertString(parameterTypeStr);

        parameters.emplace_back(parameterName, parameterType);
    }
    FunctionFlags flags {};
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<FunctionTemplate>(parent, SourceLocation(token), name, type, parameters, flags);
    UpdateStack(token, parentToken);
    Namespace::Ptr parentNamespace = dynamic_pointer_cast<Namespace>(object->Parent());
    if (parentNamespace != nullptr)
    {
        parentNamespace->Add(object);
    }
    else
    {
        // Regular type in global namespace
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr ASTCollection::AddClassTemplate(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    ClassTemplate::Ptr object;
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    bool addNewObject = true;
    if (FindClassTemplateByName(parent, name, object))
    {
        // If it already exists, we have a duplicate with a new token, and the same object with the new token
        cout << "Class template already exists." << endl;
        addNewObject = false;
    }
    else
    {
        object = make_shared<ClassTemplate>(parent, SourceLocation(token), name, accessSpecifier);
    }
    UpdateStack(token, parentToken);
    if (addNewObject)
    {
        Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
        if (parentContainer != nullptr)
        {
            parentContainer->Add(object);
        } else
        {
            Add(object);
        }
    }
    AddToMap(token, object);
    return object;
}

void ASTCollection::AddTemplateTypeParameter(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    FunctionTemplate::Ptr functionTemplate = dynamic_pointer_cast<FunctionTemplate>(parent);
    if (functionTemplate != nullptr)
    {
        functionTemplate->AddTemplateParameter(name);
        return;
    }
    ClassTemplate::Ptr classTemplate = dynamic_pointer_cast<ClassTemplate>(parent);
    if (classTemplate != nullptr)
    {
        classTemplate->AddTemplateParameter(name);
        return;
    }
    cerr << "Panic! No function or class template" << endl;
}

void ASTCollection::ShowInfo()
{
    cout << "AST collection stack contents:" << endl;
    for (size_t index = 0; index < _stack.Count(); ++index)
    {
        CXCursor element = _stack.At(_stack.Count() - index - 1);
        CXCursorKind kind = clang_getCursorKind(element);
        CXString strKind = clang_getCursorKindSpelling(kind);
        CXType type = clang_getCursorType(element);
        CXString strType = clang_getTypeSpelling(type);
        CXType underlyingType = clang_getTypedefDeclUnderlyingType(element);
        CXString strUnderlyingType = clang_getTypeSpelling(underlyingType);
        CXString strName = clang_getCursorSpelling(element);
        if (kind < CXCursorKind::CXCursor_FirstInvalid)
        {
            switch (type.kind)
            {
                case CXTypeKind::CXType_Invalid:
                    std::cout << setw(3) << index << " " << strKind << ": " << strName << std::endl;
                    break;
                case CXTypeKind::CXType_Typedef:
                    std::cout << setw(3) << index << " " << strKind << ": " << strName << ": " << strUnderlyingType << std::endl;
                    break;
                default:
                    std::cout << setw(3) << index << " " << strKind << ": " << strName << ": " << strType << std::endl;
                    break;
            }
        }
        else
        {
            std::cout << setw(3) << index << " " << strKind << ": " << strType << ": " << std::endl;
        }
    }
    cout << "AST collection token map:" << endl;
    for (auto element : _tokenLookupMap)
    {
        std::string token = ConvertString(clang_getCursorSpelling(element.first));
        cout << token << " @ " << SourceLocation(element.first) << endl;
//        cout << token << " : ";
//        TreeInfo treeInfo(cout);
//        element.second->TraverseBegin(treeInfo);
//        element.second->TraverseEnd(treeInfo);
    }
}

void ASTCollection::UpdateStack(CXCursor token, CXCursor parentToken)
{
    ssize_t index = _stack.Find(parentToken);
    if (index > 0)
    {
        // Make sure parent cursor is at top of stack, remove any others
        _stack.RemoveTopElements(index);
    }
    else if (index < 0)
    {
        // Parent is not on stack, clear stack completely
        // Make sure parent cursor is at top of stack, remove any others
        _stack.RemoveTopElements(_stack.Count());
    }
    _stack.Push(token);
}

} // namespace CPPParser
