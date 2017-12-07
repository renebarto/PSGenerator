#include <gtest/gtest.h>

#include <iostream>
#include <include/CodeGenerator.h>
#include <include/TestData.h>

using namespace std;

namespace CPPParser {
namespace Test {

class CodeGeneratorTest
    : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(CodeGeneratorTest, EmptyAST)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected = "";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(CodeGeneratorTest, SingleNamespace)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Namespace>(Declaration::WeakPtr(), "NS"));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "namespace NS {\n"
        "} // namespace NS\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(CodeGeneratorTest, SingleNamespaceAnonymous)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Namespace>(Declaration::WeakPtr(), ""));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "namespace {\n"
        "} // namespace <anonymous>\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(CodeGeneratorTest, SingleClass)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Class>(Declaration::WeakPtr(), "A", AccessSpecifier::Public));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "class A {\n"
        "}; // class A\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

//TEST_F(CodeGeneratorTest, SingleClassWithMethods)
//{
//    std::ostringstream stream;
//    CodeGenerator visitor(stream);
//
//    AST ast;
//    auto aClass = make_shared<Class>(Declaration::WeakPtr(), "A", AccessSpecifier::Public);
//    aClass->Add(make_shared<Constructor>(aClass, "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
//    aClass->Add(make_shared<Destructor>(aClass, "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
//    aClass->Add(make_shared<Method>(aClass, "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::None));
//    ast.Add(aClass);
//
//    EXPECT_TRUE(ast.Visit(visitor));
//
//}
//
TEST_F(CodeGeneratorTest, SingleStruct)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Struct>(Declaration::WeakPtr(), "A", AccessSpecifier::Public));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "struct A {\n"
        "}; // struct A\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

//TEST_F(CodeGeneratorTest, SingleStructWithMethods)
//{
//    std::ostringstream stream;
//    CodeGenerator visitor(stream);
//
//    AST ast;
//    auto aStruct = make_shared<Struct>(Declaration::WeakPtr(), "A", AccessSpecifier::Public);
//    aStruct->Add(make_shared<Constructor>(aStruct, "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
//    aStruct->Add(make_shared<Destructor>(aStruct, "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
//    aStruct->Add(make_shared<Method>(aStruct, "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::None));
//    ast.Add(aStruct);
//
//    EXPECT_TRUE(ast.Visit(visitor));
//
//}
//
//TEST_F(CodeGeneratorTest, StructInheritance)
//{
//    std::ostringstream stream;
//    CodeGenerator visitor(stream);
//
//    AST ast;
//    auto aStructA = make_shared<Struct>(Declaration::WeakPtr(), "A", AccessSpecifier::Public);
//    aStructA->Add(make_shared<Constructor>(Declaration::WeakPtr(), "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
//    aStructA->Add(make_shared<Destructor>(Declaration::WeakPtr(), "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
//    aStructA->Add(make_shared<Method>(Declaration::WeakPtr(), "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::PureVirtual));
//    ast.Add(aStructA);
//    auto aStructB = make_shared<Struct>(Declaration::WeakPtr(), "B", AccessSpecifier::Public);
//    aStructB->Add(make_shared<Constructor>(Declaration::WeakPtr(), "B", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
//    aStructB->Add(make_shared<Destructor>(Declaration::WeakPtr(), "~B", AccessSpecifier::Public, FunctionFlags::Virtual));
//    aStructB->Add(make_shared<Method>(Declaration::WeakPtr(), "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags(FunctionFlags::Virtual | FunctionFlags::Override)));
//    aStructB->AddBase(make_shared<Inheritance>(aStructB, aStructA, AccessSpecifier::Public, false));
//    ast.Add(aStructB);
//
//    EXPECT_TRUE(ast.Visit(visitor));
//
//}

TEST_F(CodeGeneratorTest, SingleEnum)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    auto aEnum = make_shared<Enum>(Declaration::WeakPtr(), "A", AccessSpecifier::Public);
    aEnum->AddValue("X", 1);
    ast.Add(aEnum);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "enum A {\n"
        "    X = 1,\n"
        "}; // enum A\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(CodeGeneratorTest, SingleEnumAnonymous)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    auto aEnum = make_shared<Enum>(Declaration::WeakPtr(), "", AccessSpecifier::Public);
    aEnum->AddValue("X", 1);
    ast.Add(aEnum);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "enum {\n"
        "    X = 1,\n"
        "}; // enum <anonymous>\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(CodeGeneratorTest, SingleEnumWithBaseType)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    auto aEnum = make_shared<Enum>(Declaration::WeakPtr(), "A", AccessSpecifier::Public, "short");
    aEnum->AddValue("X", 1);
    ast.Add(aEnum);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "enum A : short {\n"
        "    X = 1,\n"
        "}; // enum A\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(CodeGeneratorTest, SingleVariable)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Variable>(Declaration::WeakPtr(), "x", AccessSpecifier::Public, "const int"));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "const int x;\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(CodeGeneratorTest, SingleFunction)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Function>(Declaration::WeakPtr(), "x", "int",
                                  ParameterList{Parameter("y", "int")}, FunctionFlags::None));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "int x(int y);\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(CodeGeneratorTest, SingleFunctionStatic)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Function>(Declaration::WeakPtr(), "x", "int",
                                  ParameterList{Parameter("y", "int")}, FunctionFlags::Static));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "static int x(int y);\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(CodeGeneratorTest, SingleFunctionInline)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Function>(Declaration::WeakPtr(), "x", "int",
                                  ParameterList{Parameter("y", "int")}, FunctionFlags::Inline));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "inline int x(int y);\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

} // namespace Test
} // namespace CPPASTVisitor
