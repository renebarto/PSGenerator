#include <unittest-c++/UnitTestC++.h>
#include <include/IASTVisitor.h>
#include <include/AST.h>
#include <include/Variable.h>
#include <include/Typedef.h>
#include <include/Enum.h>
#include <include/Namespace.h>
#include <include/Class.h>
#include <include/Struct.h>
#include <include/TestData.h>

using namespace std;

namespace CPPParser {
namespace Test {

#define TRACE(x)\
{\
    cout << __func__ << "(" x ")" << endl << flush;\
}

#define TRACE2(x, y)\
{\
    cout << __func__ << "(" x ") : " << y << endl << flush;\
}

struct EnterLeave
{
    EnterLeave()
        : enter()
        , leave()
    {}
    void Enter()
    {
        enter = true;
    }
    void Leave()
    {
        leave = true;
    }
    bool enter;
    bool leave;
};

struct EnterLeaveInfo
{
    EnterLeaveInfo()
        : enter()
        , leave()
        , lastID()
    {}
    void Enter(const std::string & name)
    {
        enter = true;
        lastID = name;
    }
    void Leave()
    {
        leave = true;
    }
    bool enter;
    bool leave;
    std::string lastID;
};
class ASTVisitor : public IASTVisitor
{
public:
    ASTVisitor()
        : _ast()
        , _typedef()
        , _enumConstant()
        , _enum()
        , _constructor()
        , _destructor()
        , _method()
        , _function()
        , _functionTemplate()
        , _variable()
        , _dataMember()
        , _class()
        , _struct()
        , _namespace()
    {
    }

    virtual bool Enter(const AST &) override
    {
        TRACE("AST");
        _ast.Enter();
        return true;
    }
    virtual bool Leave(const AST &) override
    {
        TRACE("AST");
        _ast.Leave();
        return true;
    }

    virtual bool Enter(const Typedef & element) override
    {
        TRACE2("Typedef", element.Name());
        _typedef.Enter(element.Name());
        return true;
    }
    virtual bool Leave(const Typedef & element) override
    {
        TRACE2("Typedef", element.Name());
        _typedef.Leave();
        return true;
    }

    virtual bool Enter(const EnumConstant & element) override
    {
        TRACE2("EnumConstant", element.Name());
        _enumConstant.Enter(element.Name());
        return true;
    }
    virtual bool Leave(const EnumConstant & element) override
    {
        TRACE2("EnumConstant", element.Name());
        _enumConstant.Leave();
        return true;
    }

    virtual bool Enter(const Enum & element) override
    {
        TRACE2("Enum", element.Name());
        _enum.Enter(element.Name());
        return true;
    }
    virtual bool Leave(const Enum & element) override
    {
        TRACE2("Enum", element.Name());
        _enum.Leave();
        return true;
    }

    virtual bool Enter(const Constructor & element) override
    {
        TRACE2("Constructor", element.Name());
        _constructor.Enter(element.Name());
        return true;
    }
    virtual bool Leave(const Constructor & element) override
    {
        TRACE2("Constructor", element.Name());
        _constructor.Leave();
        return true;
    }

    virtual bool Enter(const Destructor & element) override
    {
        TRACE2("Destructor", element.Name());
        _destructor.Enter(element.Name());
        return true;
    }
    virtual bool Leave(const Destructor & element) override
    {
        TRACE2("Destructor", element.Name());
        _destructor.Leave();
        return true;
    }

    virtual bool Enter(const Method & element) override
    {
        TRACE2("Method", element.Name());
        _method.Enter(element.Name());
        return true;
    }
    virtual bool Leave(const Method & element) override
    {
        TRACE2("Method", element.Name());
        _method.Leave();
        return true;
    }

    virtual bool Enter(const Function & element) override
    {
        TRACE2("Function", element.Name());
        _function.Enter(element.Name());
        return true;
    }
    virtual bool Leave(const Function & element) override
    {
        TRACE2("Function", element.Name());
        _function.Leave();
        return true;
    }

    virtual bool Enter(const FunctionTemplate & element) override
    {
        TRACE2("FunctionTemplate", element.Name());
        _functionTemplate.Enter(element.Name());
        return true;
    }
    virtual bool Leave(const FunctionTemplate & element) override
    {
        TRACE2("FunctionTemplate", element.Name());
        _functionTemplate.Leave();
        return true;
    }

    virtual bool Enter(const Variable & element) override
    {
        TRACE2("Variable", element.Name());
        _variable.Enter(element.Name());
        return true;
    }
    virtual bool Leave(const Variable & element) override
    {
        TRACE2("Variable", element.Name());
        _variable.Leave();
        return true;
    }

    virtual bool Enter(const DataMember & element) override
    {
        TRACE2("DataMember", element.Name());
        _dataMember.Enter(element.Name());
        return true;
    }
    virtual bool Leave(const DataMember & element) override
    {
        TRACE2("DataMember", element.Name());
        _dataMember.Leave();
        return true;
    }

    virtual bool Enter(const Class & element) override
    {
        TRACE2("Class", element.Name());
        _class.Enter(element.Name());
        return true;
    }
    virtual bool Leave(const Class & element) override
    {
        TRACE2("Class", element.Name());
        _class.Leave();
        return true;
    }

    virtual bool Enter(const Struct & element) override
    {
        TRACE2("Struct", element.Name());
        _struct.Enter(element.Name());
        return true;
    }
    virtual bool Leave(const Struct & element) override
    {
        TRACE2("Struct", element.Name());
        _struct.Leave();
        return true;
    }

    virtual bool Enter(const Namespace & element) override
    {
        TRACE2("Namespace", element.Name());
        _namespace.Enter(element.Name());
        return true;
    }
    virtual bool Leave(const Namespace & element) override
    {
        TRACE2("Namespace", element.Name());
        _namespace.Leave();
        return true;
    }

public:
    EnterLeave _ast;
    EnterLeaveInfo _typedef;
    EnterLeaveInfo _enumConstant;
    EnterLeaveInfo _enum;
    EnterLeaveInfo _constructor;
    EnterLeaveInfo _destructor;
    EnterLeaveInfo _method;
    EnterLeaveInfo _function;
    EnterLeaveInfo _functionTemplate;
    EnterLeaveInfo _variable;
    EnterLeaveInfo _dataMember;
    EnterLeaveInfo _class;
    EnterLeaveInfo _struct;
    EnterLeaveInfo _namespace;
};

class ASTVisitorTest
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

#define AssertVisited(info, visited) { if (visited) { EXPECT_TRUE(info.enter); EXPECT_TRUE(info.leave); } else { EXPECT_FALSE(info.enter); EXPECT_FALSE(info.leave); } }

#define AssertVisitedInfo(info, visited, name) { if (visited) { EXPECT_TRUE(info.enter); EXPECT_TRUE(info.leave); EXPECT_EQ(name, info.lastID); } else { EXPECT_FALSE(info.enter); EXPECT_FALSE(info.leave); EXPECT_EQ(name, info.lastID); } }

TEST_FIXTURE(ASTVisitorTest, EmptyAST)
{
    ASTVisitor visitor;

    AST ast;

    EXPECT_TRUE(ast.Visit(visitor));

    AssertVisited(visitor._ast, true);
    AssertVisitedInfo(visitor._typedef, false, "");
    AssertVisitedInfo(visitor._enumConstant, false, "");
    AssertVisitedInfo(visitor._enum, false, "");
    AssertVisitedInfo(visitor._constructor, false, "");
    AssertVisitedInfo(visitor._destructor, false, "");
    AssertVisitedInfo(visitor._method, false, "");
    AssertVisitedInfo(visitor._function, false, "");
    AssertVisitedInfo(visitor._variable, false, "");
    AssertVisitedInfo(visitor._dataMember, false, "");
    AssertVisitedInfo(visitor._class, false, "");
    AssertVisitedInfo(visitor._struct, false, "");
    AssertVisitedInfo(visitor._namespace, false, "");
}

TEST_FIXTURE(ASTVisitorTest, SingleNamespace)
{
    ASTVisitor visitor;

    AST ast;
    ast.Add(make_shared<Namespace>(Declaration::WeakPtr(), "NS"));

    EXPECT_TRUE(ast.Visit(visitor));

    AssertVisited(visitor._ast, true);
    AssertVisitedInfo(visitor._typedef, false, "");
    AssertVisitedInfo(visitor._enumConstant, false, "");
    AssertVisitedInfo(visitor._enum, false, "");
    AssertVisitedInfo(visitor._constructor, false, "");
    AssertVisitedInfo(visitor._destructor, false, "");
    AssertVisitedInfo(visitor._method, false, "");
    AssertVisitedInfo(visitor._function, false, "");
    AssertVisitedInfo(visitor._variable, false, "");
    AssertVisitedInfo(visitor._dataMember, false, "");
    AssertVisitedInfo(visitor._class, false, "");
    AssertVisitedInfo(visitor._struct, false, "");
    AssertVisitedInfo(visitor._namespace, true, "NS");
}

TEST_FIXTURE(ASTVisitorTest, SingleClass)
{
    ASTVisitor visitor;

    AST ast;
    ast.Add(make_shared<Class>(Declaration::WeakPtr(), "A", AccessSpecifier::Public));

    EXPECT_TRUE(ast.Visit(visitor));

    AssertVisited(visitor._ast, true);
    AssertVisitedInfo(visitor._typedef, false, "");
    AssertVisitedInfo(visitor._enumConstant, false, "");
    AssertVisitedInfo(visitor._enum, false, "");
    AssertVisitedInfo(visitor._constructor, false, "");
    AssertVisitedInfo(visitor._destructor, false, "");
    AssertVisitedInfo(visitor._method, false, "");
    AssertVisitedInfo(visitor._function, false, "");
    AssertVisitedInfo(visitor._variable, false, "");
    AssertVisitedInfo(visitor._dataMember, false, "");
    AssertVisitedInfo(visitor._class, true, "A");
    AssertVisitedInfo(visitor._struct, false, "");
    AssertVisitedInfo(visitor._namespace, false, "");
}

TEST_FIXTURE(ASTVisitorTest, SingleClassWithMethods)
{
    ASTVisitor visitor;

    AST ast;
    auto aClass = make_shared<Class>(Declaration::WeakPtr(), "A", AccessSpecifier::Public);
    aClass->Add(make_shared<Constructor>(aClass, "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aClass->Add(make_shared<Destructor>(aClass, "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
    aClass->Add(make_shared<Method>(aClass, "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::None));
    ast.Add(aClass);

    EXPECT_TRUE(ast.Visit(visitor));

    AssertVisited(visitor._ast, true);
    AssertVisitedInfo(visitor._typedef, false, "");
    AssertVisitedInfo(visitor._enumConstant, false, "");
    AssertVisitedInfo(visitor._enum, false, "");
    AssertVisitedInfo(visitor._constructor, true, "A");
    AssertVisitedInfo(visitor._destructor, true, "~A");
    AssertVisitedInfo(visitor._method, true, "DoIt");
    AssertVisitedInfo(visitor._function, false, "");
    AssertVisitedInfo(visitor._variable, false, "");
    AssertVisitedInfo(visitor._dataMember, false, "");
    AssertVisitedInfo(visitor._class, true, "A");
    AssertVisitedInfo(visitor._struct, false, "");
    AssertVisitedInfo(visitor._namespace, false, "");
}

TEST_FIXTURE(ASTVisitorTest, SingleStruct)
{
    ASTVisitor visitor;

    AST ast;
    ast.Add(make_shared<Struct>(Declaration::WeakPtr(), "A", AccessSpecifier::Public));

    EXPECT_TRUE(ast.Visit(visitor));

    AssertVisited(visitor._ast, true);
    AssertVisitedInfo(visitor._typedef, false, "");
    AssertVisitedInfo(visitor._enumConstant, false, "");
    AssertVisitedInfo(visitor._enum, false, "");
    AssertVisitedInfo(visitor._constructor, false, "");
    AssertVisitedInfo(visitor._destructor, false, "");
    AssertVisitedInfo(visitor._method, false, "");
    AssertVisitedInfo(visitor._function, false, "");
    AssertVisitedInfo(visitor._variable, false, "");
    AssertVisitedInfo(visitor._dataMember, false, "");
    AssertVisitedInfo(visitor._class, false, "");
    AssertVisitedInfo(visitor._struct, true, "A");
    AssertVisitedInfo(visitor._namespace, false, "");
}

TEST_FIXTURE(ASTVisitorTest, SingleStructWithMethods)
{
    ASTVisitor visitor;

    AST ast;
    auto aStruct = make_shared<Struct>(Declaration::WeakPtr(), "A", AccessSpecifier::Public);
    aStruct->Add(make_shared<Constructor>(aStruct, "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aStruct->Add(make_shared<Destructor>(aStruct, "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
    aStruct->Add(make_shared<Method>(aStruct, "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::None));
    ast.Add(aStruct);

    EXPECT_TRUE(ast.Visit(visitor));

    AssertVisited(visitor._ast, true);
    AssertVisitedInfo(visitor._typedef, false, "");
    AssertVisitedInfo(visitor._enumConstant, false, "");
    AssertVisitedInfo(visitor._enum, false, "");
    AssertVisitedInfo(visitor._constructor, true, "A");
    AssertVisitedInfo(visitor._destructor, true, "~A");
    AssertVisitedInfo(visitor._method, true, "DoIt");
    AssertVisitedInfo(visitor._function, false, "");
    AssertVisitedInfo(visitor._variable, false, "");
    AssertVisitedInfo(visitor._dataMember, false, "");
    AssertVisitedInfo(visitor._class, false, "");
    AssertVisitedInfo(visitor._struct, true, "A");
    AssertVisitedInfo(visitor._namespace, false, "");
}

TEST_FIXTURE(ASTVisitorTest, StructInheritance)
{
    ASTVisitor visitor;

    AST ast;
    auto aStructA = make_shared<Struct>(Declaration::WeakPtr(), "A", AccessSpecifier::Public);
    aStructA->Add(make_shared<Constructor>(Declaration::WeakPtr(), "A", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aStructA->Add(make_shared<Destructor>(Declaration::WeakPtr(), "~A", AccessSpecifier::Public, FunctionFlags::Virtual));
    aStructA->Add(make_shared<Method>(Declaration::WeakPtr(), "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags::PureVirtual));
    ast.Add(aStructA);
    auto aStructB = make_shared<Struct>(Declaration::WeakPtr(), "B", AccessSpecifier::Public);
    aStructB->Add(make_shared<Constructor>(Declaration::WeakPtr(), "B", AccessSpecifier::Public, ParameterList(), FunctionFlags::Default));
    aStructB->Add(make_shared<Destructor>(Declaration::WeakPtr(), "~B", AccessSpecifier::Public, FunctionFlags::Virtual));
    aStructB->Add(make_shared<Method>(Declaration::WeakPtr(), "DoIt", AccessSpecifier::Public, "int", ParameterList{Parameter("x", "int")}, FunctionFlags(FunctionFlags::Virtual | FunctionFlags::Override)));
    aStructB->AddBase(make_shared<Inheritance>(aStructB, "A", AccessSpecifier::Public, aStructA, false));
    ast.Add(aStructB);

    EXPECT_TRUE(ast.Visit(visitor));

    AssertVisited(visitor._ast, true);
    AssertVisitedInfo(visitor._typedef, false, "");
    AssertVisitedInfo(visitor._enumConstant, false, "");
    AssertVisitedInfo(visitor._enum, false, "");
    AssertVisitedInfo(visitor._constructor, true, "B");
    AssertVisitedInfo(visitor._destructor, true, "~B");
    AssertVisitedInfo(visitor._method, true, "DoIt");
    AssertVisitedInfo(visitor._function, false, "");
    AssertVisitedInfo(visitor._variable, false, "");
    AssertVisitedInfo(visitor._dataMember, false, "");
    AssertVisitedInfo(visitor._class, false, "");
    AssertVisitedInfo(visitor._struct, true, "B");
    AssertVisitedInfo(visitor._namespace, false, "");
}

TEST_FIXTURE(ASTVisitorTest, SingleEnum)
{
    ASTVisitor visitor;

    AST ast;
    auto aEnum = make_shared<Enum>(Declaration::WeakPtr(), "A", AccessSpecifier::Public, "");
    aEnum->AddValue("X", 1);
    ast.Add(aEnum);

    EXPECT_TRUE(ast.Visit(visitor));

    AssertVisited(visitor._ast, true);
    AssertVisitedInfo(visitor._typedef, false, "");
    AssertVisitedInfo(visitor._enumConstant, true, "X");
    AssertVisitedInfo(visitor._enum, true, "A");
    AssertVisitedInfo(visitor._constructor, false, "");
    AssertVisitedInfo(visitor._destructor, false, "");
    AssertVisitedInfo(visitor._method, false, "");
    AssertVisitedInfo(visitor._function, false, "");
    AssertVisitedInfo(visitor._variable, false, "");
    AssertVisitedInfo(visitor._dataMember, false, "");
    AssertVisitedInfo(visitor._class, false, "");
    AssertVisitedInfo(visitor._struct, false, "");
    AssertVisitedInfo(visitor._namespace, false, "");
}

TEST_FIXTURE(ASTVisitorTest, SingleVariable)
{
    ASTVisitor visitor;

    AST ast;
    ast.Add(make_shared<Variable>(Declaration::WeakPtr(), "x", AccessSpecifier::Public, "const inst"));

    EXPECT_TRUE(ast.Visit(visitor));

    AssertVisited(visitor._ast, true);
    AssertVisitedInfo(visitor._typedef, false, "");
    AssertVisitedInfo(visitor._enumConstant, false, "");
    AssertVisitedInfo(visitor._enum, false, "");
    AssertVisitedInfo(visitor._constructor, false, "");
    AssertVisitedInfo(visitor._destructor, false, "");
    AssertVisitedInfo(visitor._method, false, "");
    AssertVisitedInfo(visitor._function, false, "");
    AssertVisitedInfo(visitor._variable, true, "x");
    AssertVisitedInfo(visitor._dataMember, false, "");
    AssertVisitedInfo(visitor._class, false, "");
    AssertVisitedInfo(visitor._struct, false, "");
    AssertVisitedInfo(visitor._namespace, false, "");
}

TEST_FIXTURE(ASTVisitorTest, SingleFunction)
{
    ASTVisitor visitor;

    AST ast;
    ast.Add(make_shared<Function>(Declaration::WeakPtr(), "x", "int",
                                  ParameterList{Parameter("y", "int")}, FunctionFlags::Static));

    EXPECT_TRUE(ast.Visit(visitor));

    AssertVisited(visitor._ast, true);
    AssertVisitedInfo(visitor._typedef, false, "");
    AssertVisitedInfo(visitor._enumConstant, false, "");
    AssertVisitedInfo(visitor._enum, false, "");
    AssertVisitedInfo(visitor._constructor, false, "");
    AssertVisitedInfo(visitor._destructor, false, "");
    AssertVisitedInfo(visitor._method, false, "");
    AssertVisitedInfo(visitor._function, true, "x");
    AssertVisitedInfo(visitor._variable, false, "");
    AssertVisitedInfo(visitor._dataMember, false, "");
    AssertVisitedInfo(visitor._class, false, "");
    AssertVisitedInfo(visitor._struct, false, "");
    AssertVisitedInfo(visitor._namespace, false, "");
}

} // namespace Test
} // namespace CPPASTVisitor
