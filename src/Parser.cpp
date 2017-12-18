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

CXChildVisitResult printVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    Parser * parser = reinterpret_cast<Parser *>(client_data);

    parser->HandleToken(cursor, parent);

    return CXChildVisit_Recurse;
}

Parser::Parser(const std::string & path)
    : _path(path)
    , _fileName()
    , _astCollection()
    , _ast()
    , _token()
    , _parentToken()
    , _traversalStack()
    , _tokenLookupMapTraversal()
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
    if (kind != CXCursorKind::CXCursor_MacroDefinition)
    {
        std::cout << strKind << " name: " << strName  << " type: " << strType << " access: " << accessSpecifier
                  << " parent: " << strKindParent << " name: " << strNameParent << std::endl << flush;
    }

}

void Parser::HandleToken(CXCursor token, CXCursor parentToken)
{
    _token = token;
    _parentToken = parentToken;

    Declaration::Ptr parent = _astCollection.Find(parentToken);

    CXCursorKind kind = clang_getCursorKind(token);
    PrintToken(token, parentToken);

    switch (kind)
    {
        case CXCursorKind::CXCursor_UnexposedDecl:          /*AddStruct(parent, token);*/ break;
        case CXCursorKind::CXCursor_StructDecl:             AddStruct(token, parentToken); break;
        case CXCursorKind::CXCursor_UnionDecl:              /*AddUnion(parent, token);*/ break;
        case CXCursorKind::CXCursor_ClassDecl:              AddClass(token, parentToken); break;
        case CXCursorKind::CXCursor_EnumDecl:               AddEnum(token, parentToken); break;
        case CXCursorKind::CXCursor_FieldDecl:              AddDataMember(token, parentToken); break;
        case CXCursorKind::CXCursor_EnumConstantDecl:       AddEnumValue(token, parentToken); break;
        case CXCursorKind::CXCursor_FunctionDecl:           AddFunction(token, parentToken); break;
        case CXCursorKind::CXCursor_VarDecl:                AddVariable(token, parentToken); break;
        case CXCursorKind::CXCursor_ParmDecl:               /*AddParameter(parent, token);*/ break;
        case CXCursorKind::CXCursor_ObjCInterfaceDecl:      break;
        case CXCursorKind::CXCursor_ObjCCategoryDecl:       break;
        case CXCursorKind::CXCursor_ObjCProtocolDecl:       break;
        case CXCursorKind::CXCursor_ObjCPropertyDecl:       break;
        case CXCursorKind::CXCursor_ObjCIvarDecl:           break;
        case CXCursorKind::CXCursor_ObjCInstanceMethodDecl: break;
        case CXCursorKind::CXCursor_ObjCClassMethodDecl:    break;
        case CXCursorKind::CXCursor_ObjCImplementationDecl: break;
        case CXCursorKind::CXCursor_ObjCCategoryImplDecl:   break;
        case CXCursorKind::CXCursor_TypedefDecl:            AddTypedef(token, parentToken); break;
        case CXCursorKind::CXCursor_CXXMethod:              AddMethod(token, parentToken); break;
        case CXCursorKind::CXCursor_Namespace:              AddNamespace(token, parentToken); break;
        case CXCursorKind::CXCursor_LinkageSpec:            /*AddNamespace(parent, token); */break;
        case CXCursorKind::CXCursor_Constructor:            AddConstructor(token, parentToken); break;
        case CXCursorKind::CXCursor_Destructor:             AddDestructor(token, parentToken); break;
        case CXCursorKind::CXCursor_ConversionFunction:     /*AddDestructor(parent, token); */break;
        case CXCursorKind::CXCursor_TemplateTypeParameter:  AddTemplateTypeParameter(token, parentToken); break;
        case CXCursorKind::CXCursor_NonTypeTemplateParameter: break;
        case CXCursorKind::CXCursor_TemplateTemplateParameter: break;
        case CXCursorKind::CXCursor_FunctionTemplate:       AddFunctionTemplate(token, parentToken); break;
        case CXCursorKind::CXCursor_ClassTemplate:          AddClassTemplate(token, parentToken); break;
        case CXCursorKind::CXCursor_ClassTemplatePartialSpecialization: break;
        case CXCursorKind::CXCursor_NamespaceAlias:         break;
        case CXCursorKind::CXCursor_UsingDirective:         break;
        case CXCursorKind::CXCursor_UsingDeclaration:       break;
        case CXCursorKind::CXCursor_TypeAliasDecl:          break;
        case CXCursorKind::CXCursor_ObjCSynthesizeDecl:     break;
        case CXCursorKind::CXCursor_ObjCDynamicDecl:        break;
        case CXCursorKind::CXCursor_CXXAccessSpecifier:     AddAccessSpecifier(token, parentToken); break;
        // References
        case CXCursorKind::CXCursor_ObjCSuperClassRef:      break;
        case CXCursorKind::CXCursor_ObjCProtocolRef:        break;
        case CXCursorKind::CXCursor_ObjCClassRef:           break;
        case CXCursorKind::CXCursor_TypeRef:                break;
        case CXCursorKind::CXCursor_CXXBaseSpecifier:       AddBaseClass(token, parentToken); break;
        case CXCursorKind::CXCursor_TemplateRef:            break;
        case CXCursorKind::CXCursor_NamespaceRef:           break;
        case CXCursorKind::CXCursor_MemberRef:              break;
        case CXCursorKind::CXCursor_LabelRef:               break;
        case CXCursorKind::CXCursor_OverloadedDeclRef:      break;
        case CXCursorKind::CXCursor_VariableRef:            break;
        // Error Conditions
        case CXCursorKind::CXCursor_InvalidFile:            break;
        case CXCursorKind::CXCursor_NoDeclFound:            break;
        case CXCursorKind::CXCursor_NotImplemented:         break;
        case CXCursorKind::CXCursor_InvalidCode:            break;
        // Expressions
        case CXCursorKind::CXCursor_UnexposedExpr:          break;
        case CXCursorKind::CXCursor_DeclRefExpr:            break;
        case CXCursorKind::CXCursor_MemberRefExpr:          break;
        case CXCursorKind::CXCursor_CallExpr:               break;
        case CXCursorKind::CXCursor_ObjCMessageExpr:        break;
        case CXCursorKind::CXCursor_BlockExpr:              break;
        case CXCursorKind::CXCursor_IntegerLiteral:         break;
        case CXCursorKind::CXCursor_FloatingLiteral:        break;
        case CXCursorKind::CXCursor_ImaginaryLiteral:       break;
        case CXCursorKind::CXCursor_StringLiteral:          break;
        case CXCursorKind::CXCursor_CharacterLiteral:       break;
        case CXCursorKind::CXCursor_ParenExpr:              break;
        case CXCursorKind::CXCursor_UnaryOperator:          break;
        case CXCursorKind::CXCursor_ArraySubscriptExpr:     break;
        case CXCursorKind::CXCursor_BinaryOperator:         break;
        case CXCursorKind::CXCursor_CompoundAssignOperator: break;
        case CXCursorKind::CXCursor_ConditionalOperator:    break;
        case CXCursorKind::CXCursor_CStyleCastExpr:         break;
        case CXCursorKind::CXCursor_CompoundLiteralExpr:    break;
        case CXCursorKind::CXCursor_InitListExpr:           break;
        case CXCursorKind::CXCursor_AddrLabelExpr:          break;
        case CXCursorKind::CXCursor_StmtExpr:               break;
        case CXCursorKind::CXCursor_GenericSelectionExpr:   break;
        case CXCursorKind::CXCursor_GNUNullExpr:            break;
        case CXCursorKind::CXCursor_CXXStaticCastExpr:      break;
        case CXCursorKind::CXCursor_CXXDynamicCastExpr:     break;
        case CXCursorKind::CXCursor_CXXReinterpretCastExpr: break;
        case CXCursorKind::CXCursor_CXXConstCastExpr:       break;
        case CXCursorKind::CXCursor_CXXFunctionalCastExpr:  break;
        case CXCursorKind::CXCursor_CXXTypeidExpr:          break;
        case CXCursorKind::CXCursor_CXXBoolLiteralExpr:     break;
        case CXCursorKind::CXCursor_CXXNullPtrLiteralExpr:  break;
        case CXCursorKind::CXCursor_CXXThisExpr:            break;
        case CXCursorKind::CXCursor_CXXThrowExpr:           break;
        case CXCursorKind::CXCursor_CXXNewExpr:             break;
        case CXCursorKind::CXCursor_CXXDeleteExpr:          break;
        case CXCursorKind::CXCursor_UnaryExpr:              break;
        case CXCursorKind::CXCursor_ObjCStringLiteral:      break;
        case CXCursorKind::CXCursor_ObjCEncodeExpr:         break;
        case CXCursorKind::CXCursor_ObjCSelectorExpr:       break;
        case CXCursorKind::CXCursor_ObjCProtocolExpr:       break;
        case CXCursorKind::CXCursor_ObjCBridgedCastExpr:    break;
        case CXCursorKind::CXCursor_PackExpansionExpr:      break;
        case CXCursorKind::CXCursor_SizeOfPackExpr:         break;
        case CXCursorKind::CXCursor_LambdaExpr:             break;
        case CXCursorKind::CXCursor_ObjCBoolLiteralExpr:    break;
        case CXCursorKind::CXCursor_ObjCSelfExpr:           break;
        case CXCursorKind::CXCursor_OMPArraySectionExpr:    break;
        // Statements
        case CXCursorKind::CXCursor_UnexposedStmt:          break;
        case CXCursorKind::CXCursor_LabelStmt:              break;
        case CXCursorKind::CXCursor_CompoundStmt:           break;
        case CXCursorKind::CXCursor_CaseStmt:               break;
        case CXCursorKind::CXCursor_DefaultStmt:            break;
        case CXCursorKind::CXCursor_IfStmt:                 break;
        case CXCursorKind::CXCursor_SwitchStmt:             break;
        case CXCursorKind::CXCursor_WhileStmt:              break;
        case CXCursorKind::CXCursor_DoStmt:                 break;
        case CXCursorKind::CXCursor_ForStmt:                break;
        case CXCursorKind::CXCursor_GotoStmt:               break;
        case CXCursorKind::CXCursor_IndirectGotoStmt:       break;
        case CXCursorKind::CXCursor_ContinueStmt:           break;
        case CXCursorKind::CXCursor_BreakStmt:              break;
        case CXCursorKind::CXCursor_ReturnStmt:             break;
        case CXCursorKind::CXCursor_GCCAsmStmt:             break;
        case CXCursorKind::CXCursor_ObjCAtTryStmt:          break;
        case CXCursorKind::CXCursor_ObjCAtCatchStmt:        break;
        case CXCursorKind::CXCursor_ObjCAtFinallyStmt:      break;
        case CXCursorKind::CXCursor_ObjCAtThrowStmt:        break;
        case CXCursorKind::CXCursor_ObjCAtSynchronizedStmt: break;
        case CXCursorKind::CXCursor_ObjCAutoreleasePoolStmt: break;
        case CXCursorKind::CXCursor_ObjCForCollectionStmt:  break;
        case CXCursorKind::CXCursor_CXXCatchStmt:           break;
        case CXCursorKind::CXCursor_CXXTryStmt:             break;
        case CXCursorKind::CXCursor_CXXForRangeStmt:        break;
        case CXCursorKind::CXCursor_SEHTryStmt:             break;
        case CXCursorKind::CXCursor_SEHExceptStmt:          break;
        case CXCursorKind::CXCursor_SEHFinallyStmt:         break;
        case CXCursorKind::CXCursor_MSAsmStmt:              break;
        case CXCursorKind::CXCursor_NullStmt:               break;
        case CXCursorKind::CXCursor_DeclStmt:               break;
        // OpenMP directives
        case CXCursorKind::CXCursor_OMPParallelDirective:   break;
        case CXCursorKind::CXCursor_OMPSimdDirective:       break;
        case CXCursorKind::CXCursor_OMPForDirective:        break;
        case CXCursorKind::CXCursor_OMPSectionsDirective:   break;
        case CXCursorKind::CXCursor_OMPSectionDirective:    break;
        case CXCursorKind::CXCursor_OMPSingleDirective:     break;
        case CXCursorKind::CXCursor_OMPParallelForDirective: break;
        case CXCursorKind::CXCursor_OMPParallelSectionsDirective: break;
        case CXCursorKind::CXCursor_OMPTaskDirective:       break;
        case CXCursorKind::CXCursor_OMPMasterDirective:     break;
        case CXCursorKind::CXCursor_OMPCriticalDirective:   break;
        case CXCursorKind::CXCursor_OMPTaskyieldDirective:  break;
        case CXCursorKind::CXCursor_OMPBarrierDirective:    break;
        case CXCursorKind::CXCursor_OMPTaskwaitDirective:   break;
        case CXCursorKind::CXCursor_OMPFlushDirective:      break;
        case CXCursorKind::CXCursor_SEHLeaveStmt:           break;
        case CXCursorKind::CXCursor_OMPOrderedDirective:    break;
        case CXCursorKind::CXCursor_OMPAtomicDirective:     break;
        case CXCursorKind::CXCursor_OMPForSimdDirective:    break;
        case CXCursorKind::CXCursor_OMPParallelForSimdDirective: break;
        case CXCursorKind::CXCursor_OMPTargetDirective:     break;
        case CXCursorKind::CXCursor_OMPTeamsDirective:      break;
        case CXCursorKind::CXCursor_OMPTaskgroupDirective:  break;
        case CXCursorKind::CXCursor_OMPCancellationPointDirective: break;
        case CXCursorKind::CXCursor_OMPCancelDirective:     break;
        case CXCursorKind::CXCursor_OMPTargetDataDirective: break;
        case CXCursorKind::CXCursor_OMPTaskLoopDirective:   break;
        case CXCursorKind::CXCursor_OMPTaskLoopSimdDirective: break;
        case CXCursorKind::CXCursor_OMPDistributeDirective: break;
        // Translation unit
        case CXCursorKind::CXCursor_TranslationUnit:        break;
        // Attributes
        case CXCursorKind::CXCursor_UnexposedAttr:          break;
        case CXCursorKind::CXCursor_IBActionAttr:           break;
        case CXCursorKind::CXCursor_IBOutletAttr:           break;
        case CXCursorKind::CXCursor_IBOutletCollectionAttr: break;
        case CXCursorKind::CXCursor_CXXFinalAttr:           break;
        case CXCursorKind::CXCursor_CXXOverrideAttr:        break;
        case CXCursorKind::CXCursor_AnnotateAttr:           break;
        case CXCursorKind::CXCursor_AsmLabelAttr:           break;
        case CXCursorKind::CXCursor_PackedAttr:             break;
        case CXCursorKind::CXCursor_PureAttr:               break;
        case CXCursorKind::CXCursor_ConstAttr:              break;
        case CXCursorKind::CXCursor_NoDuplicateAttr:        break;
        case CXCursorKind::CXCursor_CUDAConstantAttr:       break;
        case CXCursorKind::CXCursor_CUDADeviceAttr:         break;
        case CXCursorKind::CXCursor_CUDAGlobalAttr:         break;
        case CXCursorKind::CXCursor_CUDAHostAttr:           break;
        case CXCursorKind::CXCursor_CUDASharedAttr:         break;
        case CXCursorKind::CXCursor_VisibilityAttr:         break;
        case CXCursorKind::CXCursor_DLLExport:              break;
        case CXCursorKind::CXCursor_DLLImport:              break;
        // Preprocessing
        case CXCursorKind::CXCursor_PreprocessingDirective: break;
        case CXCursorKind::CXCursor_MacroDefinition:        break;
        case CXCursorKind::CXCursor_MacroExpansion:         break;
        case CXCursorKind::CXCursor_InclusionDirective:     AddInclude(token, parentToken); break;
        //Extra declarations
        case CXCursorKind::CXCursor_ModuleImportDecl:       break;
        case CXCursorKind::CXCursor_TypeAliasTemplateDecl:  break;
    }
}

void Parser::Show(std::ostream & stream)
{
    stream << "AST" << endl << endl;
    _astCollection.Show(stream, 0);
}

void Parser::TraverseTree(std::ostream & stream)
{
    CodeGenerator codeGenerator(stream);
    _ast.Visit(codeGenerator);
}

void Parser::AddToMap(Declaration::Ptr object)
{
    std::string qualifiedName = object->QualifiedName();
    if (!qualifiedName.empty())
        _typeLookupMap.insert({qualifiedName, object});
//    ShowTypeMap();
}

void Parser::AddNamespace(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr object = _astCollection.AddNamespace(token, parentToken);
    AddToMap(object);
//    _astCollection.ShowInfo();

    object = _ast.AddNamespace(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddClass(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr object = _astCollection.AddClass(token, parentToken);
    AddToMap(object);
//    _astCollection.ShowInfo();

    object = _ast.AddClass(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddStruct(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr object = _astCollection.AddStruct(token, parentToken);
    AddToMap(object);
//    _astCollection.ShowInfo();

    object = _ast.AddStruct(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddConstructor(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr object = _astCollection.AddConstructor(token, parentToken);
    AddToMap(object);
//    _astCollection.ShowInfo();

    object = _ast.AddConstructor(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddDestructor(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr object = _astCollection.AddDestructor(token, parentToken);
    AddToMap(object);
//    _astCollection.ShowInfo();

    object = _ast.AddDestructor(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddMethod(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr object = _astCollection.AddMethod(token, parentToken);
    AddToMap(object);
//    _astCollection.ShowInfo();

    object = _ast.AddMethod(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddDataMember(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr object = _astCollection.AddDataMember(token, parentToken);
    AddToMap(object);
//    _astCollection.ShowInfo();

    object = _ast.AddDataMember(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddEnum(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr object = _astCollection.AddEnum(token, parentToken);
    AddToMap(object);
//    _astCollection.ShowInfo();

    object = _ast.AddEnum(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddEnumValue(CXCursor token, CXCursor parentToken)
{
    _astCollection.AddEnumValue(token, parentToken);
//    _astCollection.ShowInfo();

    _ast.AddEnumValue(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddTypedef(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr object = _astCollection.AddTypedef(token, parentToken);
    AddToMap(object);
//    _astCollection.ShowInfo();

    _ast.AddTypedef(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddVariable(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr object = _astCollection.AddVariable(token, parentToken);
    AddToMap(object);
//    _astCollection.ShowInfo();

    _ast.AddVariable(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddFunction(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr object = _astCollection.AddFunction(token, parentToken);
    AddToMap(object);
//    _astCollection.ShowInfo();

    _ast.AddFunction(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddBaseClass(CXCursor token, CXCursor parentToken)
{
    CXType typeDecl = clang_getCursorType(token);
    CXType baseTypeDecl = clang_getCanonicalType(typeDecl);
    std::string baseTypeName = ConvertString(clang_getTypeSpelling(baseTypeDecl));
    auto it = _typeLookupMap.find(baseTypeName);
    Declaration::Ptr baseType = nullptr;
    if (it != _typeLookupMap.end())
        baseType = it->second;
    else
    {
        std::string strType = ConvertString(clang_getTypeSpelling(typeDecl));
        std::string strType2 = ConvertString(clang_getTypeSpelling(baseTypeDecl));
        cerr << "Undefined base type: " <<  strType << "," << strType2 << endl;
        return;
    }
    _astCollection.AddBaseClass(token, parentToken, baseType);
    _ast.AddBaseClass(token, parentToken, baseType);
}

void Parser::AddFunctionTemplate(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr object = _astCollection.AddFunctionTemplate(token, parentToken);
    AddToMap(object);
//    _astCollection.ShowInfo();

    _ast.AddFunctionTemplate(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddClassTemplate(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr object = _astCollection.AddClassTemplate(token, parentToken);
    AddToMap(object);
//    _astCollection.ShowInfo();

    _ast.AddClassTemplate(token, parentToken);
//    _ast.ShowInfo();
}

void Parser::AddTemplateTypeParameter(CXCursor token, CXCursor parentToken)
{
    _astCollection.AddTemplateTypeParameter(token, parentToken);
    _ast.AddTemplateTypeParameter(token, parentToken);
}

void Parser::AddAccessSpecifier(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = _astCollection.Find(parentToken);
    Object::Ptr object = dynamic_pointer_cast<Object>(parent);
    if (object != nullptr)
        object->SetAccessSpecifier(ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token)));
    parent = _ast.Find(parentToken);
    object = dynamic_pointer_cast<Object>(parent);
    if (object != nullptr)
        object->SetAccessSpecifier(ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token)));
}

void Parser::AddInclude(CXCursor token, CXCursor parentToken)
{

}

void Parser::ShowTypeMap()
{
    cout << "Type map:" << endl;
    for (auto element : _typeLookupMap)
    {
        std::string token = element.first;
        std::string parentToken;
        if (element.second->Parent() != nullptr)
            parentToken = element.second->Parent()->Name() + "::";
        cout << parentToken << element.second->Name() << " : " << token << endl;
//        cout << token << " : ";
//        TreeInfo treeInfo(cout);
//        element.second->TraverseBegin(treeInfo);
//        element.second->TraverseEnd(treeInfo);
    }
}

//void Parser::ShowTraversalStack()
//{
//    cout << "Traversal stack contents:" << endl;
//    for (size_t index = 0; index < _stack.Count(); ++index)
//    {
//        CXCursor element = _stack.At(index);
//        CXCursorKind kind = clang_getCursorKind(element);
//        CXString strKind = clang_getCursorKindSpelling(kind);
//        CXType type = clang_getCursorType(element);
//        CXString strType = clang_getTypeSpelling(type);
//        CXType underlyingType = clang_getTypedefDeclUnderlyingType(element);
//        CXString strUnderlyingType = clang_getTypeSpelling(underlyingType);
//        CXString strName = clang_getCursorSpelling(element);
//        if (kind < CXCursorKind::CXCursor_FirstInvalid)
//        {
//            switch (type.kind)
//            {
//                case CXTypeKind::CXType_Invalid:
//                    std::cout << setw(3) << index << " " << strKind << ": " << strName << std::endl;
//                    break;
//                case CXTypeKind::CXType_Typedef:
//                    std::cout << setw(3) << index << " " << strKind << ": " << strName << ": " << strUnderlyingType << std::endl;
//                    break;
//                default:
//                    std::cout << setw(3) << index << " " << strKind << ": " << strName << ": " << strType << std::endl;
//                    break;
//            }
//        }
//        else
//        {
//            std::cout << setw(3) << index << " " << strKind << ": " << strType << ": " << std::endl;
//        }
//
//        auto it = _tokenLookupMap.find(element);
//        Declaration::Ptr object = nullptr;
//        if (it != _tokenLookupMap.end())
//            object = it->second;
//        if (object == nullptr)
//        {
//            cout << "Panic";
////            exit(EXIT_FAILURE);
//        }
//        else
//        {
//            TreeInfo treeInfo(cout);
//            object->Visit(treeInfo);
//        }
//    }
//}

} // namespace CPPParser
