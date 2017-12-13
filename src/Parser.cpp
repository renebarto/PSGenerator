#include "include/Parser.h"

#include <fstream>
#include <iomanip>
#include <clang-c/Index.h>
#include <include/TreeInfo.h>
#include <include/CodeGenerator.h>
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

    Parser * parser = reinterpret_cast<Parser *>(client_data);

    parser->HandleToken(cursor, parent);

    return CXChildVisit_Recurse;
}

Parser::Parser(const std::string & path)
    : _path(path)
    , _fileName()
    , _ast()
    , _traversalTree()
    , _token()
    , _parentToken()
    , _stack()
    , _traversalStack()
    , _tokenLookupMap()
    , _typeLookupMap()
{

}

bool Parser::Parse(const OptionsList & options)
{
    std::string directory;
    std::string extension;
    Utility::SplitPath(_path, directory, _fileName, extension);

    const char ** args = new const char * [options.size()];
    for (size_t index = 0; index < options.size(); ++index)
    {
        args[index] = options[index].c_str();
    }
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit;
    CXErrorCode errorCode = clang_parseTranslationUnit2(
        index,
        _path.c_str(),
        args, static_cast<int>(options.size()),
        nullptr, 0,
        CXTranslationUnit_Flags::CXTranslationUnit_DetailedPreprocessingRecord,
        &unit);

    if ((errorCode != CXErrorCode::CXError_Success) || (unit == nullptr))
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

    return true;
}

void Parser::PrintToken(CXCursor token, CXCursor parentToken)
{
    CXType type = clang_getCursorType(token);
    CXCursorKind kind = clang_getCursorKind(token);
    CXCursorKind kindParent = clang_getCursorKind(parentToken);
    std::string strKind = ConvertString(clang_getCursorKindSpelling(kind));
    std::string strKindParent = ConvertString(clang_getCursorKindSpelling(kindParent));
    std::string strType = ConvertString(clang_getTypeSpelling(type));
    std::string strName = ConvertString(clang_getCursorSpelling(token));
    std::string strNameParent = ConvertString(clang_getCursorSpelling(parentToken));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    if (kind < CXCursorKind::CXCursor_FirstInvalid)
    {
        std::cout << strKind << " name: " << strName  << " type: " << strType << " access: " << accessSpecifier
                  << " parent: " << strKindParent << " name: " << strNameParent << std::endl << flush;
    }

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
    PrintToken(token, parentToken);

    switch (kind)
    {
        case CXCursorKind::CXCursor_UnexposedDecl:      /*AddStruct(parent, token);*/ break;
        case CXCursorKind::CXCursor_StructDecl:         AddStruct(parent, token); break;
        case CXCursorKind::CXCursor_UnionDecl:          /*AddUnion(parent, token);*/ break;
        case CXCursorKind::CXCursor_ClassDecl:          AddClass(parent, token); break;
        case CXCursorKind::CXCursor_EnumDecl:           AddEnum(parent, token); break;
        case CXCursorKind::CXCursor_FieldDecl:          AddDataMember(parent, token); break;
        case CXCursorKind::CXCursor_EnumConstantDecl:   AddEnumValue(parent, token); break;
        case CXCursorKind::CXCursor_FunctionDecl:       AddFunction(parent, token); break;
        case CXCursorKind::CXCursor_VarDecl:            AddVariable(parent, token); break;
        case CXCursorKind::CXCursor_ParmDecl:           /*AddParameter(parent, token);*/ break;
        case CXCursorKind::CXCursor_ObjCInterfaceDecl:  break;
        case CXCursorKind::CXCursor_ObjCCategoryDecl:   break;
        case CXCursorKind::CXCursor_ObjCProtocolDecl:   break;
        case CXCursorKind::CXCursor_ObjCPropertyDecl:   break;
        case CXCursorKind::CXCursor_ObjCIvarDecl:       break;
        case CXCursorKind::CXCursor_ObjCInstanceMethodDecl: break;
        case CXCursorKind::CXCursor_ObjCClassMethodDecl:break;
        case CXCursorKind::CXCursor_ObjCImplementationDecl: break;
        case CXCursorKind::CXCursor_ObjCCategoryImplDecl: break;
        case CXCursorKind::CXCursor_TypedefDecl:        AddTypedef(parent, token); break;
        case CXCursorKind::CXCursor_CXXMethod:          AddMethod(parent, token); break;
        case CXCursorKind::CXCursor_Namespace:          AddNamespace(parent, token); break;
        case CXCursorKind::CXCursor_LinkageSpec:        /*AddNamespace(parent, token); */break;
        case CXCursorKind::CXCursor_Constructor:        AddConstructor(parent, token); break;
        case CXCursorKind::CXCursor_Destructor:         AddDestructor(parent, token); break;
        case CXCursorKind::CXCursor_ConversionFunction: /*AddDestructor(parent, token); */break;
        case CXCursorKind::CXCursor_TemplateTypeParameter: AddTemplateTypeParameter(parent, token); break;
        case CXCursorKind::CXCursor_NonTypeTemplateParameter: break;
        case CXCursorKind::CXCursor_TemplateTemplateParameter: break;
        case CXCursorKind::CXCursor_FunctionTemplate:   AddFunctionTemplate(parent, token); break;
        case CXCursorKind::CXCursor_ClassTemplate:      AddClassTemplate(parent, token); break;
        case CXCursorKind::CXCursor_ClassTemplatePartialSpecialization: break;
        case CXCursorKind::CXCursor_NamespaceAlias:     break;
        case CXCursorKind::CXCursor_UsingDirective:     break;
        case CXCursorKind::CXCursor_UsingDeclaration:   break;
        case CXCursorKind::CXCursor_TypeAliasDecl:      break;
        case CXCursorKind::CXCursor_ObjCSynthesizeDecl: break;
        case CXCursorKind::CXCursor_ObjCDynamicDecl:    break;
        case CXCursorKind::CXCursor_CXXAccessSpecifier: AddAccessSpecifier(parent, token); break;
        // References
        case CXCursorKind::CXCursor_ObjCSuperClassRef:  break;
        case CXCursorKind::CXCursor_ObjCProtocolRef:    break;
        case CXCursorKind::CXCursor_ObjCClassRef:       break;
        case CXCursorKind::CXCursor_TypeRef:            break;
        case CXCursorKind::CXCursor_CXXBaseSpecifier:   AddBaseClass(parent, token); break;
        case CXCursorKind::CXCursor_TemplateRef:        break;
        case CXCursorKind::CXCursor_NamespaceRef:       break;
        case CXCursorKind::CXCursor_MemberRef:          break;
        case CXCursorKind::CXCursor_LabelRef:           break;
        case CXCursorKind::CXCursor_OverloadedDeclRef:  break;
        case CXCursorKind::CXCursor_VariableRef:        break;
        // Error Conditions
        case CXCursorKind::CXCursor_InvalidFile:        break;
        case CXCursorKind::CXCursor_NoDeclFound:        break;
        case CXCursorKind::CXCursor_NotImplemented:     break;
        case CXCursorKind::CXCursor_InvalidCode:        break;
        // Expressions
        case CXCursorKind::CXCursor_UnexposedExpr:      break;
        case CXCursorKind::CXCursor_DeclRefExpr:        break;
        case CXCursorKind::CXCursor_MemberRefExpr:      break;
        case CXCursorKind::CXCursor_CallExpr:           break;
        case CXCursorKind::CXCursor_ObjCMessageExpr:    break;
        case CXCursorKind::CXCursor_BlockExpr:          break;
        case CXCursorKind::CXCursor_IntegerLiteral:     break;
        case CXCursorKind::CXCursor_FloatingLiteral:    break;
        case CXCursorKind::CXCursor_ImaginaryLiteral:   break;
        case CXCursorKind::CXCursor_StringLiteral:      break;
        case CXCursorKind::CXCursor_CharacterLiteral:   break;
        case CXCursorKind::CXCursor_ParenExpr:          break;
        case CXCursorKind::CXCursor_UnaryOperator:      break;
        case CXCursorKind::CXCursor_ArraySubscriptExpr: break;
        case CXCursorKind::CXCursor_BinaryOperator:     break;
        case CXCursorKind::CXCursor_CompoundAssignOperator: break;
        case CXCursorKind::CXCursor_ConditionalOperator: break;
        case CXCursorKind::CXCursor_CStyleCastExpr:     break;
        case CXCursorKind::CXCursor_CompoundLiteralExpr: break;
        case CXCursorKind::CXCursor_InitListExpr:       break;
        case CXCursorKind::CXCursor_AddrLabelExpr:      break;
        case CXCursorKind::CXCursor_StmtExpr:           break;
        case CXCursorKind::CXCursor_GenericSelectionExpr: break;
        case CXCursorKind::CXCursor_GNUNullExpr:        break;
        case CXCursorKind::CXCursor_CXXStaticCastExpr:  break;
        case CXCursorKind::CXCursor_CXXDynamicCastExpr: break;
        case CXCursorKind::CXCursor_CXXReinterpretCastExpr: break;
        case CXCursorKind::CXCursor_CXXConstCastExpr:   break;
        case CXCursorKind::CXCursor_CXXFunctionalCastExpr: break;
        case CXCursorKind::CXCursor_CXXTypeidExpr:      break;
        case CXCursorKind::CXCursor_CXXBoolLiteralExpr: break;
        case CXCursorKind::CXCursor_CXXNullPtrLiteralExpr: break;
        case CXCursorKind::CXCursor_CXXThisExpr:        break;
        case CXCursorKind::CXCursor_CXXThrowExpr:       break;
        case CXCursorKind::CXCursor_CXXNewExpr:         break;
        case CXCursorKind::CXCursor_CXXDeleteExpr:      break;
        case CXCursorKind::CXCursor_UnaryExpr:          break;
        case CXCursorKind::CXCursor_ObjCStringLiteral:  break;
        case CXCursorKind::CXCursor_ObjCEncodeExpr:     break;
        case CXCursorKind::CXCursor_ObjCSelectorExpr:   break;
        case CXCursorKind::CXCursor_ObjCProtocolExpr:   break;
        case CXCursorKind::CXCursor_ObjCBridgedCastExpr: break;
        case CXCursorKind::CXCursor_PackExpansionExpr:  break;
        case CXCursorKind::CXCursor_SizeOfPackExpr:     break;
        case CXCursorKind::CXCursor_LambdaExpr:         break;
        case CXCursorKind::CXCursor_ObjCBoolLiteralExpr: break;
        case CXCursorKind::CXCursor_ObjCSelfExpr:       break;
        case CXCursorKind::CXCursor_OMPArraySectionExpr: break;
        // Statements
        case CXCursorKind::CXCursor_UnexposedStmt:      break;
        case CXCursorKind::CXCursor_LabelStmt:          break;
        case CXCursorKind::CXCursor_CompoundStmt:       break;
        case CXCursorKind::CXCursor_CaseStmt:           break;
        case CXCursorKind::CXCursor_DefaultStmt:        break;
        case CXCursorKind::CXCursor_IfStmt:             break;
        case CXCursorKind::CXCursor_SwitchStmt:         break;
        case CXCursorKind::CXCursor_WhileStmt:          break;
        case CXCursorKind::CXCursor_DoStmt:             break;
        case CXCursorKind::CXCursor_ForStmt:            break;
        case CXCursorKind::CXCursor_GotoStmt:           break;
        case CXCursorKind::CXCursor_IndirectGotoStmt:   break;
        case CXCursorKind::CXCursor_ContinueStmt:       break;
        case CXCursorKind::CXCursor_BreakStmt:          break;
        case CXCursorKind::CXCursor_ReturnStmt:         break;
        case CXCursorKind::CXCursor_GCCAsmStmt:         break;
        case CXCursorKind::CXCursor_ObjCAtTryStmt:      break;
        case CXCursorKind::CXCursor_ObjCAtCatchStmt:    break;
        case CXCursorKind::CXCursor_ObjCAtFinallyStmt:  break;
        case CXCursorKind::CXCursor_ObjCAtThrowStmt:    break;
        case CXCursorKind::CXCursor_ObjCAtSynchronizedStmt: break;
        case CXCursorKind::CXCursor_ObjCAutoreleasePoolStmt: break;
        case CXCursorKind::CXCursor_ObjCForCollectionStmt: break;
        case CXCursorKind::CXCursor_CXXCatchStmt:       break;
        case CXCursorKind::CXCursor_CXXTryStmt:         break;
        case CXCursorKind::CXCursor_CXXForRangeStmt:    break;
        case CXCursorKind::CXCursor_SEHTryStmt:         break;
        case CXCursorKind::CXCursor_SEHExceptStmt:      break;
        case CXCursorKind::CXCursor_SEHFinallyStmt:     break;
        case CXCursorKind::CXCursor_MSAsmStmt:          break;
        case CXCursorKind::CXCursor_NullStmt:           break;
        case CXCursorKind::CXCursor_DeclStmt:           break;
        // OpenMP directives
        case CXCursorKind::CXCursor_OMPParallelDirective: break;
        case CXCursorKind::CXCursor_OMPSimdDirective:   break;
        case CXCursorKind::CXCursor_OMPForDirective:    break;
        case CXCursorKind::CXCursor_OMPSectionsDirective: break;
        case CXCursorKind::CXCursor_OMPSectionDirective: break;
        case CXCursorKind::CXCursor_OMPSingleDirective: break;
        case CXCursorKind::CXCursor_OMPParallelForDirective: break;
        case CXCursorKind::CXCursor_OMPParallelSectionsDirective: break;
        case CXCursorKind::CXCursor_OMPTaskDirective:   break;
        case CXCursorKind::CXCursor_OMPMasterDirective: break;
        case CXCursorKind::CXCursor_OMPCriticalDirective: break;
        case CXCursorKind::CXCursor_OMPTaskyieldDirective: break;
        case CXCursorKind::CXCursor_OMPBarrierDirective: break;
        case CXCursorKind::CXCursor_OMPTaskwaitDirective: break;
        case CXCursorKind::CXCursor_OMPFlushDirective:  break;
        case CXCursorKind::CXCursor_SEHLeaveStmt:       break;
        case CXCursorKind::CXCursor_OMPOrderedDirective: break;
        case CXCursorKind::CXCursor_OMPAtomicDirective: break;
        case CXCursorKind::CXCursor_OMPForSimdDirective: break;
        case CXCursorKind::CXCursor_OMPParallelForSimdDirective: break;
        case CXCursorKind::CXCursor_OMPTargetDirective: break;
        case CXCursorKind::CXCursor_OMPTeamsDirective:  break;
        case CXCursorKind::CXCursor_OMPTaskgroupDirective: break;
        case CXCursorKind::CXCursor_OMPCancellationPointDirective: break;
        case CXCursorKind::CXCursor_OMPCancelDirective: break;
        case CXCursorKind::CXCursor_OMPTargetDataDirective: break;
        case CXCursorKind::CXCursor_OMPTaskLoopDirective: break;
        case CXCursorKind::CXCursor_OMPTaskLoopSimdDirective: break;
        case CXCursorKind::CXCursor_OMPDistributeDirective: break;
        // Translation unit
        case CXCursorKind::CXCursor_TranslationUnit:    break;
        // Attributes
        case CXCursorKind::CXCursor_UnexposedAttr:      break;
        case CXCursorKind::CXCursor_IBActionAttr:       break;
        case CXCursorKind::CXCursor_IBOutletAttr:       break;
        case CXCursorKind::CXCursor_IBOutletCollectionAttr: break;
        case CXCursorKind::CXCursor_CXXFinalAttr:       break;
        case CXCursorKind::CXCursor_CXXOverrideAttr:    break;
        case CXCursorKind::CXCursor_AnnotateAttr:       break;
        case CXCursorKind::CXCursor_AsmLabelAttr:       break;
        case CXCursorKind::CXCursor_PackedAttr:         break;
        case CXCursorKind::CXCursor_PureAttr:           break;
        case CXCursorKind::CXCursor_ConstAttr:          break;
        case CXCursorKind::CXCursor_NoDuplicateAttr:    break;
        case CXCursorKind::CXCursor_CUDAConstantAttr:   break;
        case CXCursorKind::CXCursor_CUDADeviceAttr:     break;
        case CXCursorKind::CXCursor_CUDAGlobalAttr:     break;
        case CXCursorKind::CXCursor_CUDAHostAttr:       break;
        case CXCursorKind::CXCursor_CUDASharedAttr:     break;
        case CXCursorKind::CXCursor_VisibilityAttr:     break;
        case CXCursorKind::CXCursor_DLLExport:          break;
        case CXCursorKind::CXCursor_DLLImport:          break;
        // Preprocessing
        case CXCursorKind::CXCursor_PreprocessingDirective: break;
        case CXCursorKind::CXCursor_MacroDefinition:    break;
        case CXCursorKind::CXCursor_MacroExpansion:     break;
        case CXCursorKind::CXCursor_InclusionDirective: AddInclude(parent, token); break;
        //Extra declarations
        case CXCursorKind::CXCursor_ModuleImportDecl:   break;
        case CXCursorKind::CXCursor_TypeAliasTemplateDecl: break;
    }
}

void Parser::Show(std::ostream & stream)
{
    stream << "AST" << endl << endl;
    _ast.Show(stream, 0);
}

void Parser::TraverseTree(std::ostream & stream)
{
    CodeGenerator codeGenerator(stream);
    _traversalTree.Visit(codeGenerator);
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

bool Parser::FindClassTemplateByName(Declaration::Ptr parent, const std::string & name, ClassTemplate::Ptr & result)
{
    result = {};
    if (parent == nullptr)
        return _ast.FindClassTemplate(name, result);
    auto parentContainer = dynamic_pointer_cast<Container>(parent);
    if (parentContainer != nullptr)
        return parentContainer->FindClassTemplate(name, result);
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
    _traversalTree.AddNode(make_shared<Traversal::Element>(object));
    AddToMap(token, object);
    ShowTraversalStack();
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

    FunctionFlags flags {};
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

void Parser::AddTypedef(Declaration::Ptr parent, CXCursor token)
{
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    std::string type = ConvertString(clang_getTypeSpelling(clang_getTypedefDeclUnderlyingType(token)));

    auto object = make_shared<Typedef>(parent, name, accessSpecifier, type);
    UpdateStack(object);
    Namespace::Ptr parentNamespace = dynamic_pointer_cast<Namespace>(object->Parent());
    if (parentNamespace != nullptr)
    {
        parentNamespace->Add(object);
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
    Namespace::Ptr parentNamespace = dynamic_pointer_cast<Namespace>(object->Parent());
    if (parentNamespace != nullptr)
    {
        parentNamespace->Add(object);
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

    auto object = make_shared<Function>(parent, name, type, parameters, flags);
    UpdateStack(object);
    Namespace::Ptr parentNamespace = dynamic_pointer_cast<Namespace>(object->Parent());
    if (parentNamespace != nullptr)
    {
        parentNamespace->Add(object);
    }
    else
    {
        // Regular type in global namespace
        _ast.Add(object);
    }
    AddToMap(token, object);
}

void Parser::AddAccessSpecifier(Declaration::Ptr parent, CXCursor token)
{
    Object::Ptr object = dynamic_pointer_cast<Object>(parent);
    if (object != nullptr)
        object->SetAccessSpecifier(ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token)));
}

void Parser::AddInclude(Declaration::Ptr parent, CXCursor token)
{

}

void Parser::AddFunctionTemplate(Declaration::Ptr parent, CXCursor token)
{
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

    auto object = make_shared<FunctionTemplate>(parent, name, type, parameters, flags);
    UpdateStack(object);
    Namespace::Ptr parentNamespace = dynamic_pointer_cast<Namespace>(object->Parent());
    if (parentNamespace != nullptr)
    {
        parentNamespace->Add(object);
    }
    else
    {
        // Regular type in global namespace
        _ast.Add(object);
    }
    AddToMap(token, object);
}

void Parser::AddClassTemplate(Declaration::Ptr parent, CXCursor token)
{
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
        object = make_shared<ClassTemplate>(parent, name, accessSpecifier);
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

void Parser::AddTemplateTypeParameter(Declaration::Ptr parent, CXCursor token)
{
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

    // Update traversal stack
    index = _traversalStack.Find(object);
    if (index > 0)
    {
        // Make sure parent cursor is at top of stack, remove any others
        _traversalStack.RemoveTopElements(index);
    }
    else if (index < 0)
    {
        // Parent is not on stack, clear stack completely
        // Make sure parent cursor is at top of stack, remove any others
        _traversalStack.RemoveTopElements(_traversalStack.Count());
    }
    _traversalStack.Push(object);
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
            TreeInfo treeInfo(cout);
            object->Visit(treeInfo);
        }
    }
    cout << "Token map" << endl;
    for (auto element : _tokenLookupMap)
    {
        std::string token = ConvertString(clang_getCursorSpelling(element.first));
        cout << token << " : ";
        TreeInfo treeInfo(cout);
        element.second->Visit(treeInfo);
    }
    cout << "Type map" << endl;
    for (auto element : _typeLookupMap)
    {
        std::string token = ConvertString(clang_getTypeSpelling(element.first));
        cout << token << " : ";
        TreeInfo treeInfo(cout);
        element.second->Visit(treeInfo);
    }
}

void Parser::ShowTraversalStack()
{
    cout << "Traversal stack contents:" << endl;
    TreeInfo treeInfo(cout);
    _ast.TraverseBegin(treeInfo);
    for (size_t index = 0; index < _traversalStack.Count(); ++index)
    {
        Declaration::Ptr element = _traversalStack.At(index);
        element->TraverseBegin(treeInfo);
        element->TraverseEnd(treeInfo);
    }
    _ast.TraverseEnd(treeInfo);
}

} // namespace CPPParser
