#include <unittest-c++/UnitTestC++.h>

#include <iostream>
#include <include/CodeGenerator.h>
#include <include/TestData.h>

using namespace std;

namespace CPPParser {
namespace Test {

class CodeGeneratorTest
    : public ::UnitTestCpp::TestFixture
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_FIXTURE(CodeGeneratorTest, EmptyAST)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected = "";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, SingleNamespace)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Namespace>(Element::WeakPtr(), SourceLocation(), "NS"));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "namespace NS {\n"
        "} // namespace NS\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, SingleNamespaceAnonymous)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Namespace>(Element::WeakPtr(), SourceLocation(), ""));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "namespace {\n"
        "} // namespace <anonymous>\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, SingleClass)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Class>(Element::WeakPtr(), SourceLocation(), "A", AccessSpecifier::Public));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "class A {\n"
        "}; // class A\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, SingleClassWithMethods)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    auto aClass = make_shared<Class>(Element::WeakPtr(), SourceLocation(), "A", AccessSpecifier::Public);
    aClass->Add(make_shared<Constructor>(aClass, SourceLocation(), "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aClass->Add(make_shared<Destructor>(aClass, SourceLocation(), "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
    aClass->Add(make_shared<Method>(aClass, SourceLocation(), "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::None));
    ast.Add(aClass);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "class A {\n"
        "    A() = default;\n"
        "    virtual ~A();\n"
        "    int DoIt(int x);\n"
        "}; // class A\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, SingleClassWithMethodsAndVariables)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    auto aClass = make_shared<Class>(Element::WeakPtr(), SourceLocation(), "A", AccessSpecifier::Public);
    aClass->Add(make_shared<Constructor>(aClass, SourceLocation(), "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aClass->Add(make_shared<Destructor>(aClass, SourceLocation(), "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
    aClass->Add(make_shared<Method>(aClass, SourceLocation(), "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::None));
    aClass->Add(make_shared<DataMember>(aClass, SourceLocation(), "X", AccessSpecifier::Public, "int"));
    ast.Add(aClass);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "class A {\n"
            "    A() = default;\n"
            "    virtual ~A();\n"
            "    int DoIt(int x);\n"
            "    int X;\n"
            "}; // class A\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, SingleStruct)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Struct>(Element::WeakPtr(), SourceLocation(), "A", AccessSpecifier::Public));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "struct A {\n"
        "}; // struct A\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, SingleStructWithMethods)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    auto aStruct = make_shared<Struct>(Element::WeakPtr(), SourceLocation(), "A", AccessSpecifier::Public);
    aStruct->Add(make_shared<Constructor>(aStruct, SourceLocation(), "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aStruct->Add(make_shared<Destructor>(aStruct, SourceLocation(), "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
    aStruct->Add(make_shared<Method>(aStruct, SourceLocation(), "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::None));
    ast.Add(aStruct);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "struct A {\n"
        "    A() = default;\n"
        "    virtual ~A();\n"
        "    int DoIt(int x);\n"
        "}; // struct A\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, StructInheritance)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    auto aStructA = make_shared<Struct>(Element::WeakPtr(), SourceLocation(), "A", AccessSpecifier::Public);
    aStructA->Add(make_shared<Constructor>(aStructA, SourceLocation(), "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aStructA->Add(make_shared<Destructor>(aStructA, SourceLocation(), "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
    aStructA->Add(make_shared<Method>(aStructA, SourceLocation(), "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::PureVirtual));
    ast.Add(aStructA);
    auto aStructB = make_shared<Struct>(Element::WeakPtr(), SourceLocation(), "B", AccessSpecifier::Public);
    aStructB->Add(make_shared<Constructor>(aStructB, SourceLocation(), "B", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aStructB->Add(make_shared<Destructor>(aStructB, SourceLocation(), "~B", AccessSpecifier::Public, FunctionFlags::Virtual));
    aStructB->Add(make_shared<Method>(aStructB, SourceLocation(), "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags(FunctionFlags::Virtual | FunctionFlags::Override)));
    aStructB->AddBase(make_shared<Inheritance>(aStructB, SourceLocation(), "A", AccessSpecifier::Public, aStructA, false));
    ast.Add(aStructB);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "struct A {\n"
        "    A() = default;\n"
        "    virtual ~A();\n"
        "    virtual int DoIt(int x) = 0;\n"
        "}; // struct A\n"
        "struct B : public A {\n"
        "    B() = default;\n"
        "    virtual ~B();\n"
        "    virtual int DoIt(int x) override;\n"
        "}; // struct B\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, SingleEnum)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    auto aEnum = make_shared<Enum>(Element::WeakPtr(), SourceLocation(), "A", AccessSpecifier::Public, "");
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

TEST_FIXTURE(CodeGeneratorTest, SingleEnumAnonymous)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    auto aEnum = make_shared<Enum>(Element::WeakPtr(), SourceLocation(), "", AccessSpecifier::Public, "");
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

TEST_FIXTURE(CodeGeneratorTest, SingleEnumWithBaseType)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    auto aEnum = make_shared<Enum>(Element::WeakPtr(), SourceLocation(), "A", AccessSpecifier::Public, "short");
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

TEST_FIXTURE(CodeGeneratorTest, SingleVariable)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Variable>(Element::WeakPtr(), SourceLocation(), "x", AccessSpecifier::Public, "const int"));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "const int x;\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, SingleFunction)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Function>(Element::WeakPtr(), SourceLocation(), "x", "int",
                                  ParameterList{Parameter("y", "int")}, FunctionFlags::None));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "int x(int y);\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, SingleFunctionStatic)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Function>(Element::WeakPtr(), SourceLocation(), "x", "int",
                                  ParameterList{Parameter("y", "int")}, FunctionFlags::Static));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "static int x(int y);\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, SingleFunctionInline)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Function>(Element::WeakPtr(), SourceLocation(), "x", "int",
                                  ParameterList{Parameter("y", "int")}, FunctionFlags::Inline));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "inline int x(int y);\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, SingleTypedef)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<Typedef>(Element::WeakPtr(), SourceLocation(), "x", AccessSpecifier::Invalid, "int"));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "typedef int x;\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, IncludeDirectiveLocal)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<IncludeDirective>(Element::WeakPtr(), SourceLocation(), "Module.h", IncludeSpecifier::Local));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "#include \"Module.h\"\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CodeGeneratorTest, IncludeDirectiveSystem)
{
    std::ostringstream stream;
    CodeGenerator visitor(stream);

    AST ast;
    ast.Add(make_shared<IncludeDirective>(Element::WeakPtr(), SourceLocation(), "iostream", IncludeSpecifier::System));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "#include <iostream>\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

} // namespace Test
} // namespace CPPASTVisitor
