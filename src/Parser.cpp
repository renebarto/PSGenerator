#include "include/Parser.h"

#include <fstream>
#include <iomanip>
#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Typedef.h"
#include "include/Variable.h"

using namespace std;
using namespace Utility;

namespace CPPParser
{

ostream & operator << (ostream & stream, const CXString & str)
{
    stream << clang_getCString(str);
    return stream;
}

CXChildVisitResult printVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    CXCursor cursorObject = clang_getCursorReferenced(cursor);
    CXCursor parentObject = clang_getCursorReferenced(parent);

    CXType type = clang_getCursorType(cursorObject);
    CXCursorKind kind = clang_getCursorKind(cursorObject);
    std::string strKind = ConvertString(clang_getCursorKindSpelling(kind));
    std::string strType = ConvertString(clang_getTypeSpelling(type));
    std::string strName = ConvertString(clang_getCursorSpelling(cursorObject));
    Parser * parser = reinterpret_cast<Parser *>(client_data);

    if (kind < CXCursorKind::CXCursor_FirstInvalid)
    {
//        std::cout << "Declaration " << strKind << "\n" << "type is: " << strType << std::endl << flush;
//        std::cout << "name is: " << strName << std::endl << flush;
    }

    parser->HandleToken(cursor, parent);

    return CXChildVisit_Recurse;
}

Parser::Parser(const std::string & path)
    : _path(path)
    , _fileName()
    , _ast()
    , _token()
    , _parentToken()
    , _stack()
    , _tokenLookupMap()
    , _typeLookupMap()
{

}

bool Parser::Parse(const OptionsList & options)
{
    fstream stream(_path);

    std::string directory;
    std::string extension;
    Utility::SplitPath(_path, directory, _fileName, extension);

    const char ** args = new const char * [options.size()];
    for (size_t index = 0; index < options.size(); ++index)
    {
        args[index] = options[index].c_str();
    }
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_createTranslationUnitFromSourceFile(
        index,
        _path.c_str(),
        static_cast<int>(options.size()), args,
        0, nullptr);

    if (unit == nullptr)
    {
        cerr << "Unable to parse translation unit. Quitting." << endl;
        return false;
    }
    if (clang_getNumDiagnostics(unit) > 0)
    {
        for (size_t i = 0; i < clang_getNumDiagnostics(unit); ++i)
        {
            CXDiagnostic diagnostic = clang_getDiagnostic(unit, i);
            cerr << ConvertString(clang_getDiagnosticSpelling(diagnostic)) << endl;
        }
    }

    delete [] args;
    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(cursor, printVisitor, this);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    Show(cout);

    return true;
}

void Parser::HandleToken(CXCursor token, CXCursor parentToken)
{
    _token = token;
    _parentToken = parentToken;
    auto it = _tokenLookupMap.find(parentToken);
    Declaration::Ptr parent = nullptr;
    if (it != _tokenLookupMap.end())
        parent = it->second;

    CXCursorKind kind = clang_getCursorKind(token);

    switch (kind)
    {
        case CXCursorKind::CXCursor_EnumDecl:           AddEnum(parent, token); break;
        case CXCursorKind::CXCursor_EnumConstantDecl:   AddEnumValue(parent, token); break;
        case CXCursorKind::CXCursor_ClassDecl:          AddClass(parent, token); break;
        case CXCursorKind::CXCursor_StructDecl:         AddStruct(parent, token); break;
        case CXCursorKind::CXCursor_Namespace:          AddNamespace(parent, token); break;
        case CXCursorKind::CXCursor_Constructor:        AddConstructor(parent, token); break;
        case CXCursorKind::CXCursor_Destructor:         AddDestructor(parent, token); break;
        case CXCursorKind::CXCursor_CXXMethod:          AddMethod(parent, token); break;
        case CXCursorKind::CXCursor_CXXBaseSpecifier:   AddBaseClass(parent, token); break;
        case CXCursorKind::CXCursor_TypedefDecl:        AddTypedef(parent, token); break;
    }
}

void Parser::Show(std::ostream & stream)
{
    stream << "AST" << endl << endl;
    _ast.Show(stream, 0);
}

void Parser::AddToMap(CXCursor token, Declaration::Ptr object)
{
    CXType type = clang_getCursorType(token);
    if (type.kind != 0)
        _typeLookupMap.insert({type, object});
    _tokenLookupMap.insert({token, object});
}

bool Parser::FindNamespaceByName(Declaration::Ptr parent, const std::string & name, Namespace::Ptr & result)
{
    result = {};
    if (parent == nullptr)
        return _ast.FindNamespace(name, result);
    auto parentContainer = dynamic_pointer_cast<Container>(parent);
    if (parentContainer != nullptr)
        return parentContainer->FindNamespace(name, result);
    return false;
}

bool Parser::FindClassByName(Declaration::Ptr parent, const std::string & name, Class::Ptr & result)
{
    result = {};
    if (parent == nullptr)
        return _ast.FindClass(name, result);
    auto parentContainer = dynamic_pointer_cast<Container>(parent);
    if (parentContainer != nullptr)
        return parentContainer->FindClass(name, result);
    return false;
}

bool Parser::FindStructByName(Declaration::Ptr parent, const std::string & name, Struct::Ptr & result)
{
    result = {};
    if (parent == nullptr)
        return _ast.FindStruct(name, result);
    auto parentContainer = dynamic_pointer_cast<Container>(parent);
    if (parentContainer != nullptr)
        return parentContainer->FindStruct(name, result);
    return false;
}

bool Parser::FindEnumByName(Declaration::Ptr parent, const std::string & name, Enum::Ptr & result)
{
    result = {};
    if (parent == nullptr)
        return _ast.FindEnum(name, result);
    auto parentContainer = dynamic_pointer_cast<Container>(parent);
    if (parentContainer != nullptr)
        return parentContainer->FindEnum(name, result);
    return false;
}

void Parser::AddNamespace(Declaration::Ptr parent, CXCursor token)
{
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
        std::string name = ConvertString(clang_getCursorSpelling(token));
        object = make_shared<Namespace>(parent, name);
    }
    UpdateStack(object);
    if (addNewObject)
    {
        Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
        if (parentContainer != nullptr)
        {
            parentContainer->Add(object);
        }
        else
        {
            _ast.Add(object);
        }
    }
    AddToMap(token, object);
}

void Parser::AddClass(Declaration::Ptr parent, CXCursor token)
{
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
        object = make_shared<Class>(parent, name, accessSpecifier);
    }
    UpdateStack(object);
    if (addNewObject)
    {
        Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
        if (parentContainer != nullptr)
        {
            parentContainer->Add(object);
        } else
        {
            _ast.Add(object);
        }
    }
    AddToMap(token, object);
}

void Parser::AddStruct(Declaration::Ptr parent, CXCursor token)
{
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
        object = make_shared<Struct>(parent, name, accessSpecifier);
    }
    UpdateStack(object);
    if (addNewObject)
    {
        Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
        if (parentContainer != nullptr)
        {
            parentContainer->Add(object);
        } else
        {
            _ast.Add(object);
        }
    }
    AddToMap(token, object);
}

void Parser::AddEnum(Declaration::Ptr parent, CXCursor token)
{
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    std::string underlyingType;
    CXType type = clang_getEnumDeclIntegerType(token);
    if (type.kind != CXType_UInt)
    {
        CXString strType = clang_getTypeSpelling(type);
        underlyingType = ConvertString(strType);
    }

    auto object = make_shared<Enum>(parent, name, accessSpecifier, underlyingType);
    UpdateStack(object);
    Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
    if (parentContainer != nullptr)
    {
        parentContainer->Add(object);
    }
    else
    {
        _ast.Add(object);
    }
    AddToMap(token, object);
}

void Parser::AddEnumValue(Declaration::Ptr parent, CXCursor token)
{
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

void Parser::AddConstructor(Declaration::Ptr parent, CXCursor token)
{
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    CXType functionType = clang_getCursorType(token);

    int numArguments = clang_Cursor_getNumArguments(token);
    ParameterList parameters;
    for (unsigned int i = 0; i < numArguments; ++i)
    {
        CXCursor parameterToken = clang_Cursor_getArgument(token, i);
        CXString parameterNameStr = clang_getCursorSpelling(parameterToken);
        CXType parameterTypeDecl = clang_getArgType(functionType, i);
        CXString parameterTypeStr = clang_getTypeSpelling(parameterTypeDecl);
        std::string parameterName = ConvertString(parameterNameStr);
        std::string parameterType = ConvertString(parameterTypeStr);

        parameters.emplace_back(parameterName, parameterType);
    }
    FunctionFlags flags;
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<Constructor>(parent, name, accessSpecifier, parameters, flags);
    UpdateStack(object);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        cerr << "Parent is not an object" << endl;
        return;
    }
    AddToMap(token, object);
}

void Parser::AddDestructor(Declaration::Ptr parent, CXCursor token)
{
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    CXType functionType = clang_getCursorType(token);
    CXType resultType = clang_getResultType(functionType);
    CXString resultTypeStr = clang_getTypeSpelling(resultType);
    std::string type = ConvertString(resultTypeStr);

    FunctionFlags flags;
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<Destructor>(parent, name, accessSpecifier, flags);
    UpdateStack(object);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        cerr << "Parent is not an object" << endl;
        return;
    }
    AddToMap(token, object);
}

void Parser::AddMethod(Declaration::Ptr parent, CXCursor token)
{
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    CXType functionType = clang_getCursorType(token);
    CXType resultType = clang_getResultType(functionType);
    CXString resultTypeStr = clang_getTypeSpelling(resultType);
    std::string type = ConvertString(resultTypeStr);

    int numArguments = clang_Cursor_getNumArguments(token);
    ParameterList parameters;
    for (unsigned int i = 0; i < numArguments; ++i)
    {
        CXCursor parameterToken = clang_Cursor_getArgument(token, i);
        CXString parameterNameStr = clang_getCursorSpelling(parameterToken);
        CXType parameterTypeDecl = clang_getArgType(functionType, i);
        CXString parameterTypeStr = clang_getTypeSpelling(parameterTypeDecl);
        std::string parameterName = ConvertString(parameterNameStr);
        std::string parameterType = ConvertString(parameterTypeStr);

        parameters.emplace_back(parameterName, parameterType);
    }
    FunctionFlags flags;
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<Method>(parent, name, accessSpecifier, type, parameters, flags);
    UpdateStack(object);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        // Regular function in global namespace
        _ast.Add(object);
    }
    AddToMap(token, object);
}

void Parser::AddBaseClass(Declaration::Ptr parent, CXCursor token)
{
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    bool isVirtual = (clang_isVirtualBase(token) != 0);

    CXType typeDecl = clang_getCursorType(token);
    CXType baseTypeDecl = clang_getCanonicalType(typeDecl);

    auto it = _typeLookupMap.find(baseTypeDecl);
    Declaration::Ptr baseType = nullptr;
    if (it != _typeLookupMap.end())
        baseType = it->second;
    else
    {
        std::string strType = ConvertString(clang_getTypeSpelling(typeDecl));
        cerr << "Undefined base type: " <<  strType << endl;
        return;
    }
    auto inheritance = make_shared<Inheritance>(parent, name, accessSpecifier, baseType, isVirtual);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(parent);
    if (parentObject != nullptr)
    {
        parentObject->AddBase(inheritance);
    }
    else
    {
        cerr << "Type is not an object" <<  parent->Name() << endl;
        return;
    }
}

void Parser::AddTypedef(Declaration::Ptr parent, CXCursor token)
{
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    std::string type = ConvertString(clang_getTypeSpelling(clang_getTypedefDeclUnderlyingType(token)));

    auto object = make_shared<Typedef>(parent, name, accessSpecifier, type);
    UpdateStack(object);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        // Regular type in global namespace
        _ast.Add(object);
    }
    AddToMap(token, object);
}

void Parser::AddVariable(Declaration::Ptr parent, CXCursor token)
{
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    std::string type = ConvertString(clang_getTypeSpelling(clang_getCursorType(token)));

    auto object = make_shared<Variable>(parent, name, accessSpecifier, type);
    UpdateStack(object);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        // Regular type in global namespace
        _ast.Add(object);
    }
    AddToMap(token, object);
}

void Parser::AddDataMember(Declaration::Ptr parent, CXCursor token)
{
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    std::string type = ConvertString(clang_getTypeSpelling(clang_getCursorType(token)));

    auto object = make_shared<DataMember>(parent, name, accessSpecifier, type);
    UpdateStack(object);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        // Regular type in global namespace
        _ast.Add(object);
    }
    AddToMap(token, object);
}

void Parser::AddFunction(Declaration::Ptr parent, CXCursor token)
{
    std::string name = ConvertString(clang_getCursorSpelling(token));
    CXType functionType = clang_getCursorType(token);
    CXType resultType = clang_getResultType(functionType);
    CXString resultTypeStr = clang_getTypeSpelling(resultType);
    std::string type = ConvertString(resultTypeStr);

    int numArguments = clang_Cursor_getNumArguments(token);
    ParameterList parameters;
    for (unsigned int i = 0; i < numArguments; ++i)
    {
        CXCursor parameterToken = clang_Cursor_getArgument(token, i);
        CXString parameterNameStr = clang_getCursorSpelling(parameterToken);
        CXType parameterTypeDecl = clang_getArgType(functionType, i);
        CXString parameterTypeStr = clang_getTypeSpelling(parameterTypeDecl);
        std::string parameterName = ConvertString(parameterNameStr);
        std::string parameterType = ConvertString(parameterTypeStr);

        parameters.emplace_back(parameterName, parameterType);
    }
    FunctionFlags flags;
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<Function>(parent, name, type, parameters, flags);
    UpdateStack(object);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        // Regular type in global namespace
        _ast.Add(object);
    }
    AddToMap(token, object);
}

void Parser::UpdateStack(Declaration::Ptr object)
{
    ssize_t index = _stack.Find(_parentToken);
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
    _stack.Push(_token);
}

void Parser::ShowStack()
{
    cout << "Stack contents:" << endl;
    for (size_t index = 0; index < _stack.Count(); ++index)
    {
        CXCursor element = _stack.At(index);
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

        auto it = _tokenLookupMap.find(element);
        Declaration::Ptr object = nullptr;
        if (it != _tokenLookupMap.end())
            object = it->second;
        if (object == nullptr)
        {
            cout << "Panic";
//            exit(EXIT_FAILURE);
        }
        else
        {
            object->Show(cout, 0);
        }
    }
    cout << "Token map" << endl;
    for (auto element : _tokenLookupMap)
    {
        std::string token = ConvertString(clang_getCursorSpelling(element.first));
        cout << token << " : ";
        element.second->Show(cout, 0);
    }
    cout << "Type map" << endl;
    for (auto element : _typeLookupMap)
    {
        std::string token = ConvertString(clang_getTypeSpelling(element.first));
        cout << token << " : ";
        element.second->Show(cout, 0);
    }
}

} // namespace CPPParser
