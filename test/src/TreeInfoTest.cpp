#include <unittest-c++/UnitTestC++.h>

#include <iostream>
#include <include/TreeInfo.h>
#include <include/TestData.h>

using namespace std;

namespace CPPParser {
namespace Test {

class TreeInfoTest
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

TEST_FIXTURE(TreeInfoTest, EmptyAST)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleNamespace)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    ast.Add(make_shared<Namespace>(Declaration::WeakPtr(), "NS"));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "    Namespace end <global>\n"
        "    Namespace NS\n"
        "    Namespace end NS\n"
        "    Namespace <global>\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleNamespaceAnonymous)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    ast.Add(make_shared<Namespace>(Declaration::WeakPtr(), ""));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "    Namespace end <global>\n"
        "    Namespace <anonymous>\n"
        "    Namespace end <anonymous>\n"
        "    Namespace <global>\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleClass)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    ast.Add(make_shared<Class>(Declaration::WeakPtr(), "A", AccessSpecifier::Public));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "        Class A\n"
        "        Class end A\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleClassWithMethods)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    auto aClass = make_shared<Class>(Declaration::WeakPtr(), "A", AccessSpecifier::Public);
    aClass->Add(make_shared<Constructor>(aClass, "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aClass->Add(make_shared<Destructor>(aClass, "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
    aClass->Add(make_shared<Method>(aClass, "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::None));
    ast.Add(aClass);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "        Class A\n"
        "            Constructor A: \n"
        "                Qualifiers: default\n"
        "                Parameters:\n"
        "            Constructor end A\n"
        "            Destructor ~A: \n"
        "                Qualifiers: virtual\n"
        "                Parameters:\n"
        "            Destructor end ~A\n"
        "            Method DoIt: int\n"
        "                Qualifiers:\n"
        "                Parameters:\n"
        "                    x: int\n"
        "            Method end DoIt\n"
        "        Class end A\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleClassWithMethodsAndVariables)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    auto aClass = make_shared<Class>(Declaration::WeakPtr(), "A", AccessSpecifier::Public);
    aClass->Add(make_shared<Constructor>(aClass, "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aClass->Add(make_shared<Destructor>(aClass, "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
    aClass->Add(make_shared<Method>(aClass, "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::None));
    aClass->Add(make_shared<DataMember>(aClass, "X", AccessSpecifier::Public, "int"));
    ast.Add(aClass);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
            "    Namespace <global>\n"
            "        Class A\n"
            "            Constructor A: \n"
            "                Qualifiers: default\n"
            "                Parameters:\n"
            "            Constructor end A\n"
            "            Destructor ~A: \n"
            "                Qualifiers: virtual\n"
            "                Parameters:\n"
            "            Destructor end ~A\n"
            "            Method DoIt: int\n"
            "                Qualifiers:\n"
            "                Parameters:\n"
            "                    x: int\n"
            "            Method end DoIt\n"
            "            Member variable X: int\n"
            "        Class end A\n"
            "    Namespace end <global>\n"
            "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleStruct)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    ast.Add(make_shared<Struct>(Declaration::WeakPtr(), "A", AccessSpecifier::Public));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "        Struct A\n"
        "        Struct end A\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleStructWithMethods)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    auto aStruct = make_shared<Struct>(Declaration::WeakPtr(), "A", AccessSpecifier::Public);
    aStruct->Add(make_shared<Constructor>(aStruct, "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aStruct->Add(make_shared<Destructor>(aStruct, "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
    aStruct->Add(make_shared<Method>(aStruct, "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::None));
    ast.Add(aStruct);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "        Struct A\n"
        "            Constructor A: \n"
        "                Qualifiers: default\n"
        "                Parameters:\n"
        "            Constructor end A\n"
        "            Destructor ~A: \n"
        "                Qualifiers: virtual\n"
        "                Parameters:\n"
        "            Destructor end ~A\n"
        "            Method DoIt: int\n"
        "                Qualifiers:\n"
        "                Parameters:\n"
        "                    x: int\n"
        "            Method end DoIt\n"
        "        Struct end A\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, StructInheritance)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    auto aStructA = make_shared<Struct>(Declaration::WeakPtr(), "A", AccessSpecifier::Public);
    aStructA->Add(make_shared<Constructor>(aStructA, "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aStructA->Add(make_shared<Destructor>(aStructA, "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
    aStructA->Add(make_shared<Method>(aStructA, "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::PureVirtual));
    ast.Add(aStructA);
    auto aStructB = make_shared<Struct>(Declaration::WeakPtr(), "B", AccessSpecifier::Public);
    aStructB->Add(make_shared<Constructor>(aStructB, "B", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aStructB->Add(make_shared<Destructor>(aStructB, "~B", AccessSpecifier::Public, FunctionFlags::Virtual));
    aStructB->Add(make_shared<Method>(aStructB, "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags(FunctionFlags::Virtual | FunctionFlags::Override)));
    aStructB->AddBase(make_shared<Inheritance>(aStructB, aStructA, AccessSpecifier::Public, false));
    ast.Add(aStructB);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "        Struct A\n"
        "            Constructor A: \n"
        "                Qualifiers: default\n"
        "                Parameters:\n"
        "            Constructor end A\n"
        "            Destructor ~A: \n"
        "                Qualifiers: virtual\n"
        "                Parameters:\n"
        "            Destructor end ~A\n"
        "            Method DoIt: int\n"
        "                Qualifiers: virtual purevirtual\n"
        "                Parameters:\n"
        "                    x: int\n"
        "            Method end DoIt\n"
        "        Struct end A\n"
        "        Struct B\n"
        "            Inheritance: \n"
        "                A: public\n"
        "            Constructor B: \n"
        "                Qualifiers: default\n"
        "                Parameters:\n"
        "            Constructor end B\n"
        "            Destructor ~B: \n"
        "                Qualifiers: virtual\n"
        "                Parameters:\n"
        "            Destructor end ~B\n"
        "            Method DoIt: int\n"
        "                Qualifiers: virtual override\n"
        "                Parameters:\n"
        "                    x: int\n"
        "            Method end DoIt\n"
        "        Struct end B\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleEnum)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    auto aEnum = make_shared<Enum>(Declaration::WeakPtr(), "A", AccessSpecifier::Public);
    aEnum->AddValue("X", 1);
    ast.Add(aEnum);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "        Enum A: <default base>\n"
        "            EnumConstant X = 1\n"
        "        Enum end A\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleEnumAnonymous)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    auto aEnum = make_shared<Enum>(Declaration::WeakPtr(), "", AccessSpecifier::Public);
    aEnum->AddValue("X", 1);
    ast.Add(aEnum);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "        Enum <anonymous>: <default base>\n"
        "            EnumConstant X = 1\n"
        "        Enum end <anonymous>\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleEnumWithBaseType)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    auto aEnum = make_shared<Enum>(Declaration::WeakPtr(), "A", AccessSpecifier::Public, "short");
    aEnum->AddValue("X", 1);
    ast.Add(aEnum);

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "        Enum A: short\n"
        "            EnumConstant X = 1\n"
        "        Enum end A\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleVariable)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    ast.Add(make_shared<Variable>(Declaration::WeakPtr(), "x", AccessSpecifier::Public, "const int"));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "        Variable x: const int\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleFunction)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    ast.Add(make_shared<Function>(Declaration::WeakPtr(), "x", "int",
                                  ParameterList{Parameter("y", "int")}, FunctionFlags::None));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "        Function x: int\n"
        "            Qualifiers:\n"
        "            Parameters:\n"
        "                y: int\n"
        "        Function end x\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleFunctionStatic)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    ast.Add(make_shared<Function>(Declaration::WeakPtr(), "x", "int",
                                  ParameterList{Parameter("y", "int")}, FunctionFlags::Static));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "        Function x: int\n"
        "            Qualifiers: static\n"
        "            Parameters:\n"
        "                y: int\n"
        "        Function end x\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleFunctionInline)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    ast.Add(make_shared<Function>(Declaration::WeakPtr(), "x", "int",
                                  ParameterList{Parameter("y", "int")}, FunctionFlags::Inline));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "        Function x: int\n"
        "            Qualifiers: inline\n"
        "            Parameters:\n"
        "                y: int\n"
        "        Function end x\n"
        "    Namespace end <global>\n"
        "AST end\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TreeInfoTest, SingleTypedef)
{
    std::ostringstream stream;
    TreeInfo visitor(stream);

    AST ast;
    ast.Add(make_shared<Typedef>(Declaration::WeakPtr(), "x", AccessSpecifier::Invalid, "int"));

    EXPECT_TRUE(ast.Visit(visitor));

    std::string expected =
        "AST begin\n"
        "    Namespace <global>\n"
        "        Typedef x: int\n"
        "    Namespace end <global>\n"
        "AST end\n";
        "typedef int x;\n";
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

} // namespace Test
} // namespace CPPASTVisitor
