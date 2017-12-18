#include <unittest-c++/UnitTestC++.h>
#include <include/Parser.h>
#include <include/TestData.h>
#include <include/CodeGenerator.h>

namespace CPPParser {
namespace Test {

class ParserTest : public ::UnitTestCpp::TestFixture {
protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

static OptionsList compileOptions =
    {
        "-x",
        "c++",
        "-std=c++11",
    };

TEST_FIXTURE(ParserTest, Empty)
{
    Parser parser(TestData::EmptyHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{0}, astCollection.Namespaces().size());
    EXPECT_EQ(size_t{0}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{0}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    std::string expected =
        "";
    std::ostringstream stream;
    astCollection.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected = "";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, SingleNamespace)
{
    Parser parser(TestData::SingleNamespaceHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{1}, astCollection.Namespaces().size());
    EXPECT_EQ(size_t{0}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{0}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr ns = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, ns);
    EXPECT_EQ("NS1", ns->Name());
    EXPECT_EQ(nullptr, ns->Parent());

    EXPECT_EQ(size_t{0}, ns->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns->Classes().size());
    EXPECT_EQ(size_t{0}, ns->Structs().size());
    EXPECT_EQ(size_t{0}, ns->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns->Enums().size());
    EXPECT_EQ(size_t{0}, ns->Functions().size());
    EXPECT_EQ(size_t{0}, ns->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns->Variables().size());
    EXPECT_EQ(size_t{0}, ns->FunctionTemplates().size());

    EXPECT_EQ("namespace NS1", ns->QualifiedDescription());

    std::string expected =
        "namespace NS1 {\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    astCollection.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace NS1 {\n"
        "} // namespace NS1\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, NestedNamespace)
{
    Parser parser(TestData::NestedNamespaceHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{1}, astCollection.Namespaces().size());
    EXPECT_EQ(size_t{0}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{0}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr ns1 = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());
    EXPECT_EQ(size_t{0}, ns1->Functions().size());
    EXPECT_EQ(size_t{0}, ns1->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns1->Variables().size());
    EXPECT_EQ(size_t{0}, ns1->FunctionTemplates().size());

    EXPECT_EQ("namespace NS1", ns1->QualifiedDescription());

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("NS2", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns2->Classes().size());
    EXPECT_EQ(size_t{0}, ns2->Structs().size());
    EXPECT_EQ(size_t{0}, ns2->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns2->Enums().size());
    EXPECT_EQ(size_t{0}, ns2->Functions().size());
    EXPECT_EQ(size_t{0}, ns2->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns2->Variables().size());
    EXPECT_EQ(size_t{0}, ns2->FunctionTemplates().size());

    EXPECT_EQ("namespace NS1::NS2", ns2->QualifiedDescription());

    std::string expected =
        "namespace NS1 {\n"
        "    namespace NS2 {\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    astCollection.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace NS1 {\n"
        "    namespace NS2 {\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, AnonymousNamespace)
{
    Parser parser(TestData::AnonymousNamespaceHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{1}, astCollection.Namespaces().size());
    EXPECT_EQ(size_t{0}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{0}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr ns1 = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());
    EXPECT_EQ(size_t{0}, ns1->Functions().size());
    EXPECT_EQ(size_t{0}, ns1->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns1->Variables().size());
    EXPECT_EQ(size_t{0}, ns1->FunctionTemplates().size());

    EXPECT_EQ("namespace NS1", ns1->QualifiedDescription());

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns2->Classes().size());
    EXPECT_EQ(size_t{0}, ns2->Structs().size());
    EXPECT_EQ(size_t{0}, ns2->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns2->Enums().size());
    EXPECT_EQ(size_t{0}, ns2->Functions().size());
    EXPECT_EQ(size_t{0}, ns2->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns2->Variables().size());
    EXPECT_EQ(size_t{0}, ns2->FunctionTemplates().size());

    EXPECT_EQ("namespace NS1::<anonymous>", ns2->QualifiedDescription());

    std::string expected =
        "namespace NS1 {\n"
        "    namespace {\n"
        "    } // namespace <anonymous>\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    astCollection.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace NS1 {\n"
        "    namespace {\n"
        "    } // namespace <anonymous>\n"
        "} // namespace NS1\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, NamespaceWithVarsAndFunctions)
{
    Parser parser(TestData::NamespaceWithVarsAndFunctionsHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{1}, astCollection.Namespaces().size());
    EXPECT_EQ(size_t{0}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{0}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr ns1 = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ(size_t{0}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());
    EXPECT_EQ(size_t{1}, ns1->Functions().size());
    EXPECT_EQ(size_t{1}, ns1->Typedefs().size());
    EXPECT_EQ(size_t{1}, ns1->Variables().size());
    EXPECT_EQ(size_t{0}, ns1->FunctionTemplates().size());

    EXPECT_EQ("int NS1::foo(double)", ns1->Functions()[0]->QualifiedDescription());
    EXPECT_EQ("typedef float NS1::z", ns1->Typedefs()[0]->QualifiedDescription());
    EXPECT_EQ("long NS1::y", ns1->Variables()[0]->QualifiedDescription());

    std::string expected =
        "namespace NS1 {\n"
        "    int foo(double x);\n"
        "    long y;\n"
        "    typedef float z;\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    astCollection.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace NS1 {\n"
        "    int foo(double x);\n"
        "    long y;\n"
        "    typedef float z;\n"
        "} // namespace NS1\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, Class)
{
    Parser parser(TestData::ClassHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{1}, astCollection.Namespaces().size());
    EXPECT_EQ(size_t{0}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{0}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr ns1 = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{1}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());
    EXPECT_EQ(size_t{0}, ns1->Functions().size());
    EXPECT_EQ(size_t{0}, ns1->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns1->Variables().size());
    EXPECT_EQ(size_t{0}, ns1->FunctionTemplates().size());

    EXPECT_EQ("namespace NS1", ns1->QualifiedDescription());

    Class::Ptr classDef = ns1->Classes()[0];
    ASSERT_NE(nullptr, classDef);
    EXPECT_EQ("interface", classDef->Name());
    EXPECT_EQ(ns1, classDef->Parent());

    EXPECT_EQ(size_t{0}, classDef->Namespaces().size());
    EXPECT_EQ(size_t{0}, classDef->Classes().size());
    EXPECT_EQ(size_t{0}, classDef->Structs().size());
    EXPECT_EQ(size_t{0}, classDef->Enums().size());
    EXPECT_EQ(size_t{1}, classDef->Constructors().size());
    EXPECT_EQ(size_t{1}, classDef->Destructors().size());
    EXPECT_EQ(size_t{1}, classDef->Methods().size());
    EXPECT_EQ(size_t{0}, classDef->DataMembers().size());

    EXPECT_EQ("class NS1::interface", classDef->QualifiedDescription());
    EXPECT_EQ("NS1::interface::interface()", classDef->Constructors()[0]->QualifiedDescription());
    EXPECT_EQ("NS1::interface::~interface()", classDef->Destructors()[0]->QualifiedDescription());
    EXPECT_EQ("void NS1::interface::callme()", classDef->Methods()[0]->QualifiedDescription());

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("NS2", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{1}, ns2->Classes().size());
    EXPECT_EQ(size_t{0}, ns2->Structs().size());
    EXPECT_EQ(size_t{0}, ns2->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns2->Enums().size());
    EXPECT_EQ(size_t{0}, ns2->Functions().size());
    EXPECT_EQ(size_t{0}, ns2->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns2->Variables().size());
    EXPECT_EQ(size_t{0}, ns2->FunctionTemplates().size());

    EXPECT_EQ("namespace NS1::NS2", ns2->QualifiedDescription());

    classDef = ns2->Classes()[0];
    EXPECT_EQ("c", classDef->Name());
    EXPECT_EQ(ns2, classDef->Parent());

    EXPECT_EQ(size_t{0}, classDef->Namespaces().size());
    EXPECT_EQ(size_t{0}, classDef->Classes().size());
    EXPECT_EQ(size_t{0}, classDef->Structs().size());
    EXPECT_EQ(size_t{0}, classDef->Enums().size());
    EXPECT_EQ(size_t{1}, classDef->Constructors().size());
    EXPECT_EQ(size_t{1}, classDef->Destructors().size());
    EXPECT_EQ(size_t{1}, classDef->Methods().size());
    EXPECT_EQ(size_t{1}, classDef->DataMembers().size());

    EXPECT_EQ("class NS1::NS2::c", classDef->QualifiedDescription());
    EXPECT_EQ("NS1::NS2::c::c(const NS1::interface *)", classDef->Constructors()[0]->QualifiedDescription());
    EXPECT_EQ("NS1::NS2::c::~c()", classDef->Destructors()[0]->QualifiedDescription());
    EXPECT_EQ("const NS1::interface * NS1::NS2::c::X()", classDef->Methods()[0]->QualifiedDescription());
    EXPECT_EQ("const NS1::interface * NS1::NS2::c::_interface", classDef->DataMembers()[0]->QualifiedDescription());

    std::string expected =
        "namespace NS1 {\n"
        "    class interface {\n"
        "        interface();\n"
        "        virtual ~interface();\n"
        "        virtual void callme() = 0;\n"
        "    }; // class interface\n"
        "    namespace NS2 {\n"
        "        class c {\n"
        "            c(const NS1::interface * i);\n"
        "            virtual ~c();\n"
        "            const NS1::interface * X() const;\n"
        "            const NS1::interface * _interface;\n"
        "        }; // class c\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    astCollection.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace NS1 {\n"
        "    class interface {\n"
        "        interface();\n"
        "        virtual ~interface();\n"
        "        virtual void callme() = 0;\n"
        "    }; // class interface\n"
        "    namespace NS2 {\n"
        "        class c {\n"
        "            c(const NS1::interface * i);\n"
        "            virtual ~c();\n"
        "            const NS1::interface * X() const;\n"
        "            const NS1::interface * _interface;\n"
        "        }; // class c\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, Struct)
{
    Parser parser(TestData::StructHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{1}, astCollection.Namespaces().size());
    EXPECT_EQ(size_t{0}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{0}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr ns1 = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ("namespace NS1", ns1->QualifiedDescription());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());
    EXPECT_EQ(size_t{0}, ns1->Functions().size());
    EXPECT_EQ(size_t{0}, ns1->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns1->Variables().size());
    EXPECT_EQ(size_t{0}, ns1->FunctionTemplates().size());

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("NS2", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns2->Classes().size());
    EXPECT_EQ(size_t{2}, ns2->Structs().size());
    EXPECT_EQ(size_t{0}, ns2->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns2->Enums().size());
    EXPECT_EQ(size_t{0}, ns2->Functions().size());
    EXPECT_EQ(size_t{0}, ns2->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns2->Variables().size());
    EXPECT_EQ(size_t{0}, ns2->FunctionTemplates().size());

    EXPECT_EQ("namespace NS1::NS2", ns2->QualifiedDescription());

    Struct::Ptr structDef = ns2->Structs()[0];
    ASSERT_NE(nullptr, structDef);
    EXPECT_EQ("interface", structDef->Name());
    EXPECT_EQ(ns2, structDef->Parent());

    EXPECT_EQ(size_t{0}, structDef->Namespaces().size());
    EXPECT_EQ(size_t{0}, structDef->Classes().size());
    EXPECT_EQ(size_t{0}, structDef->Structs().size());
    EXPECT_EQ(size_t{0}, structDef->Enums().size());
    EXPECT_EQ(size_t{1}, structDef->Constructors().size());
    EXPECT_EQ(size_t{1}, structDef->Destructors().size());
    EXPECT_EQ(size_t{1}, structDef->Methods().size());
    EXPECT_EQ(size_t{0}, structDef->DataMembers().size());

    EXPECT_EQ("struct NS1::NS2::interface", structDef->QualifiedDescription());
    EXPECT_EQ("NS1::NS2::interface::interface()", structDef->Constructors()[0]->QualifiedDescription());
    EXPECT_EQ("NS1::NS2::interface::~interface()", structDef->Destructors()[0]->QualifiedDescription());
    EXPECT_EQ("void NS1::NS2::interface::callme()", structDef->Methods()[0]->QualifiedDescription());

    structDef = ns2->Structs()[1];
    ASSERT_NE(nullptr, structDef);
    EXPECT_EQ("s", structDef->Name());
    EXPECT_EQ(ns2, structDef->Parent());

    EXPECT_EQ(size_t{0}, structDef->Namespaces().size());
    EXPECT_EQ(size_t{0}, structDef->Classes().size());
    EXPECT_EQ(size_t{0}, structDef->Structs().size());
    EXPECT_EQ(size_t{0}, structDef->Enums().size());
    EXPECT_EQ(size_t{1}, structDef->Constructors().size());
    EXPECT_EQ(size_t{1}, structDef->Destructors().size());
    EXPECT_EQ(size_t{1}, structDef->Methods().size());
    EXPECT_EQ(size_t{1}, structDef->DataMembers().size());

    EXPECT_EQ("struct NS1::NS2::s", structDef->QualifiedDescription());
    EXPECT_EQ("NS1::NS2::s::s(const NS1::NS2::interface *)", structDef->Constructors()[0]->QualifiedDescription());
    EXPECT_EQ("NS1::NS2::s::~s()", structDef->Destructors()[0]->QualifiedDescription());
    EXPECT_EQ("const NS1::NS2::interface * NS1::NS2::s::X()", structDef->Methods()[0]->QualifiedDescription());
    EXPECT_EQ("const NS1::NS2::interface * NS1::NS2::s::_interface", structDef->DataMembers()[0]->QualifiedDescription());

    std::string expected =
        "namespace NS1 {\n"
        "    namespace NS2 {\n"
        "        struct interface {\n"
        "            interface();\n"
        "            virtual ~interface();\n"
        "            virtual void callme() = 0;\n"
        "        }; // struct interface\n"
        "        struct s {\n"
        "            s(const NS1::NS2::interface * i);\n"
        "            virtual ~s();\n"
        "            const NS1::NS2::interface * X() const;\n"
        "            const NS1::NS2::interface * _interface;\n"
        "        }; // struct s\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    astCollection.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace NS1 {\n"
        "    namespace NS2 {\n"
        "        struct interface {\n"
        "            interface();\n"
        "            virtual ~interface();\n"
        "            virtual void callme() = 0;\n"
        "        }; // struct interface\n"
        "        struct s {\n"
        "            s(const NS1::NS2::interface * i);\n"
        "            virtual ~s();\n"
        "            const NS1::NS2::interface * X() const;\n"
        "            const NS1::NS2::interface * _interface;\n"
        "        }; // struct s\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, Enum)
{
    Parser parser(TestData::EnumHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{1}, astCollection.Namespaces().size());
    EXPECT_EQ(size_t{0}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{0}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr ns1 = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());
    EXPECT_EQ(size_t{0}, ns1->Functions().size());
    EXPECT_EQ(size_t{0}, ns1->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns1->Variables().size());
    EXPECT_EQ(size_t{0}, ns1->FunctionTemplates().size());

    EXPECT_EQ("namespace NS1", ns1->QualifiedDescription());

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("NS2", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ("namespace NS1::NS2", ns2->QualifiedDescription());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns2->Classes().size());
    EXPECT_EQ(size_t{0}, ns2->Structs().size());
    EXPECT_EQ(size_t{0}, ns2->ClassTemplates().size());
    EXPECT_EQ(size_t{1}, ns2->Enums().size());
    EXPECT_EQ(size_t{0}, ns2->Functions().size());
    EXPECT_EQ(size_t{0}, ns2->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns2->Variables().size());
    EXPECT_EQ(size_t{0}, ns2->FunctionTemplates().size());

    EXPECT_EQ("enum NS1::NS2::e", ns2->Enums()[0]->QualifiedDescription());

    const Enum::Ptr enumeration = ns2->Enums()[0];
    ASSERT_NE(nullptr, enumeration);
    EXPECT_EQ("e", enumeration->Name());
    EXPECT_EQ(ns2, enumeration->Parent());
    EXPECT_EQ(size_t {2}, enumeration->Values().size());

    std::string expected =
        "namespace NS1 {\n"
        "    namespace NS2 {\n"
        "        enum e : short {\n"
        "            a = 0,\n"
        "            b = 1,\n"
        "        }; // enum e\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    CodeGenerator codeGenerator(stream);
    astCollection.Visit(codeGenerator);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace NS1 {\n"
        "    namespace NS2 {\n"
        "        enum e : short {\n"
        "            a = 0,\n"
        "            b = 1,\n"
        "        }; // enum e\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, EnumAnonymous)
{
    Parser parser(TestData::EnumAnonymousHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{1}, astCollection.Namespaces().size());
    EXPECT_EQ(size_t{0}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{0}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr ns1 = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ("namespace NS1", ns1->QualifiedDescription());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());
    EXPECT_EQ(size_t{0}, ns1->Functions().size());
    EXPECT_EQ(size_t{0}, ns1->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns1->Variables().size());
    EXPECT_EQ(size_t{0}, ns1->FunctionTemplates().size());

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("NS2", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ("namespace NS1::NS2", ns2->QualifiedDescription());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns2->Classes().size());
    EXPECT_EQ(size_t{0}, ns2->Structs().size());
    EXPECT_EQ(size_t{0}, ns2->ClassTemplates().size());
    EXPECT_EQ(size_t{1}, ns2->Enums().size());
    EXPECT_EQ(size_t{0}, ns2->Functions().size());
    EXPECT_EQ(size_t{0}, ns2->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns2->Variables().size());
    EXPECT_EQ(size_t{0}, ns2->FunctionTemplates().size());

    EXPECT_EQ("enum NS1::NS2::<anonymous>", ns2->Enums()[0]->QualifiedDescription());

    const Enum::Ptr enumeration = ns2->Enums()[0];
    ASSERT_NE(nullptr, enumeration);
    EXPECT_EQ("", enumeration->Name());
    EXPECT_EQ(ns2, enumeration->Parent());

    std::string expected =
        "namespace NS1 {\n"
        "    namespace NS2 {\n"
        "        enum {\n"
        "            a = 0,\n"
        "            b = 1,\n"
        "        }; // enum <anonymous>\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    CodeGenerator codeGenerator(stream);
    astCollection.Visit(codeGenerator);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace NS1 {\n"
        "    namespace NS2 {\n"
        "        enum {\n"
        "            a = 0,\n"
        "            b = 1,\n"
        "        }; // enum <anonymous>\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, Inheritance)
{
    Parser parser(TestData::InheritanceHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{1}, astCollection.Namespaces().size());
    EXPECT_EQ(size_t{0}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{0}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr ns1 = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ("namespace NS1", ns1->QualifiedDescription());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());
    EXPECT_EQ(size_t{0}, ns1->Functions().size());
    EXPECT_EQ(size_t{0}, ns1->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns1->Variables().size());
    EXPECT_EQ(size_t{0}, ns1->FunctionTemplates().size());

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("NS2", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ("namespace NS1::NS2", ns2->QualifiedDescription());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{2}, ns2->Classes().size());
    EXPECT_EQ(size_t{0}, ns2->Structs().size());
    EXPECT_EQ(size_t{0}, ns2->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns2->Enums().size());
    EXPECT_EQ(size_t{0}, ns2->Functions().size());
    EXPECT_EQ(size_t{0}, ns2->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns2->Variables().size());
    EXPECT_EQ(size_t{0}, ns2->FunctionTemplates().size());

    const Class::Ptr A = ns2->Classes()[0];
    ASSERT_NE(nullptr, A);
    EXPECT_EQ("A", A->Name());
    EXPECT_EQ(ns2, A->Parent());

    const Class::Ptr B = ns2->Classes()[1];
    ASSERT_NE(nullptr, B);
    EXPECT_EQ("B", B->Name());
    EXPECT_EQ(ns2, B->Parent());

    EXPECT_EQ("class NS1::NS2::A", ns2->Classes()[0]->QualifiedDescription());
    EXPECT_EQ("class NS1::NS2::B", ns2->Classes()[1]->QualifiedDescription());

    std::string expected =
        "namespace NS1 {\n"
        "    namespace NS2 {\n"
        "        class A {\n"
        "            A();\n"
        "            virtual ~A();\n"
        "            virtual void DoIt() = 0;\n"
        "        }; // class A\n"
        "        class B : virtual public class NS1::NS2::A {\n"
        "            B();\n"
        "            virtual ~B();\n"
        "            virtual void DoIt();\n"
        "        }; // class B\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    astCollection.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace NS1 {\n"
        "    namespace NS2 {\n"
        "        class A {\n"
        "            A();\n"
        "            virtual ~A();\n"
        "            virtual void DoIt() = 0;\n"
        "        }; // class A\n"
        "        class B : virtual public class NS1::NS2::A {\n"
        "            B();\n"
        "            virtual ~B();\n"
        "            virtual void DoIt();\n"
        "        }; // class B\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, FunctionTemplate)
{
    Parser parser(TestData::TemplateFunctionHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{3}, astCollection.Namespaces().size());
    EXPECT_EQ(size_t{0}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{0}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr nsstd = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, nsstd);
    EXPECT_EQ("std", nsstd->Name());
    EXPECT_EQ(nullptr, nsstd->Parent());

    EXPECT_EQ("namespace std", nsstd->QualifiedDescription());

    EXPECT_EQ(size_t{1}, nsstd->Namespaces().size());
    EXPECT_EQ(size_t{0}, nsstd->Classes().size());
    EXPECT_EQ(size_t{0}, nsstd->Structs().size());
    EXPECT_EQ(size_t{0}, nsstd->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsstd->Enums().size());
    EXPECT_EQ(size_t{0}, nsstd->Functions().size());
    EXPECT_EQ(size_t{3}, nsstd->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsstd->Variables().size());
    EXPECT_EQ(size_t{0}, nsstd->FunctionTemplates().size());

    const Namespace::Ptr nsGNUCXX = astCollection.Namespaces()[1];
    ASSERT_NE(nullptr, nsGNUCXX);
    EXPECT_EQ("__gnu_cxx", nsGNUCXX->Name());
    EXPECT_EQ(nullptr, nsGNUCXX->Parent());

    EXPECT_EQ("namespace __gnu_cxx", nsGNUCXX->QualifiedDescription());

    EXPECT_EQ(size_t{1}, nsGNUCXX->Namespaces().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->Classes().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->Structs().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->Enums().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->Functions().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->Variables().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->FunctionTemplates().size());

    const Namespace::Ptr ns1 = astCollection.Namespaces()[2];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ("namespace NS1", ns1->QualifiedDescription());

    EXPECT_EQ(size_t{0}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());
    EXPECT_EQ(size_t{0}, ns1->Functions().size());
    EXPECT_EQ(size_t{0}, ns1->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns1->Variables().size());
    EXPECT_EQ(size_t{1}, ns1->FunctionTemplates().size());

    EXPECT_EQ("template<class T> std::size_t NS1::Size()", ns1->FunctionTemplates()[0]->QualifiedDescription());

    std::string expected =
        "namespace std {\n"
        "    typedef unsigned long size_t;\n"
        "    typedef long ptrdiff_t;\n"
        "    typedef decltype(nullptr) nullptr_t;\n"
        "    namespace __cxx11 {\n"
        "    } // namespace __cxx11\n"
        "} // namespace std\n"
        "namespace __gnu_cxx {\n"
        "    namespace __cxx11 {\n"
        "    } // namespace __cxx11\n"
        "} // namespace __gnu_cxx\n"
        "namespace NS1 {\n"
        "    template<class T> std::size_t Size();\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    astCollection.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace std {\n"
        "    typedef unsigned long size_t;\n"
        "    typedef long ptrdiff_t;\n"
        "    typedef decltype(nullptr) nullptr_t;\n"
        "    namespace __cxx11 {\n"
        "    } // namespace __cxx11\n"
        "} // namespace std\n"
        "namespace __gnu_cxx {\n"
        "    namespace __cxx11 {\n"
        "    } // namespace __cxx11\n"
        "} // namespace __gnu_cxx\n"
        "namespace std {\n"
        "} // namespace std\n"
        "namespace NS1 {\n"
        "    template<class T> std::size_t Size();\n"
        "} // namespace NS1\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, ClassTemplate)
{
    Parser parser(TestData::TemplateClassHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{3}, astCollection.Namespaces().size());
    EXPECT_EQ(size_t{0}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{0}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr nsstd = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, nsstd);
    EXPECT_EQ("std", nsstd->Name());
    EXPECT_EQ(nullptr, nsstd->Parent());

    EXPECT_EQ("namespace std", nsstd->QualifiedDescription());

    EXPECT_EQ(size_t{1}, nsstd->Namespaces().size());
    EXPECT_EQ(size_t{0}, nsstd->Classes().size());
    EXPECT_EQ(size_t{0}, nsstd->Structs().size());
    EXPECT_EQ(size_t{0}, nsstd->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsstd->Enums().size());
    EXPECT_EQ(size_t{0}, nsstd->Functions().size());
    EXPECT_EQ(size_t{3}, nsstd->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsstd->Variables().size());
    EXPECT_EQ(size_t{0}, nsstd->FunctionTemplates().size());

    const Namespace::Ptr nsGNUCXX = astCollection.Namespaces()[1];
    ASSERT_NE(nullptr, nsGNUCXX);
    EXPECT_EQ("__gnu_cxx", nsGNUCXX->Name());
    EXPECT_EQ(nullptr, nsGNUCXX->Parent());

    EXPECT_EQ("namespace __gnu_cxx", nsGNUCXX->QualifiedDescription());

    EXPECT_EQ(size_t{1}, nsGNUCXX->Namespaces().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->Classes().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->Structs().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->Enums().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->Functions().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->Variables().size());
    EXPECT_EQ(size_t{0}, nsGNUCXX->FunctionTemplates().size());

    const Namespace::Ptr ns1 = astCollection.Namespaces()[2];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ("namespace NS1", ns1->QualifiedDescription());

    EXPECT_EQ(size_t{0}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{1}, ns1->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());
    EXPECT_EQ(size_t{0}, ns1->Functions().size());
    EXPECT_EQ(size_t{0}, ns1->Typedefs().size());
    EXPECT_EQ(size_t{0}, ns1->Variables().size());
    EXPECT_EQ(size_t{0}, ns1->FunctionTemplates().size());

    auto classDef = ns1->ClassTemplates()[0];
    EXPECT_EQ(size_t{0}, classDef->Namespaces().size());
    EXPECT_EQ(size_t{0}, classDef->Classes().size());
    EXPECT_EQ(size_t{0}, classDef->Structs().size());
    EXPECT_EQ(size_t{0}, classDef->Enums().size());
    EXPECT_EQ(size_t{1}, classDef->Constructors().size());
    EXPECT_EQ(size_t{0}, classDef->Destructors().size());
    EXPECT_EQ(size_t{1}, classDef->Methods().size());
    EXPECT_EQ(size_t{1}, classDef->DataMembers().size());

    EXPECT_EQ("class template<class T> NS1::A", classDef->QualifiedDescription());
    EXPECT_EQ("template<class T> NS1::A::A<T>(std::size_t)", classDef->Constructors()[0]->QualifiedDescription());
    EXPECT_EQ("const T * template<class T> NS1::A::Data()", classDef->Methods()[0]->QualifiedDescription());
    EXPECT_EQ("T * template<class T> NS1::A::_data", classDef->DataMembers()[0]->QualifiedDescription());

    std::string expected =
        "namespace std {\n"
        "    typedef unsigned long size_t;\n"
        "    typedef long ptrdiff_t;\n"
        "    typedef decltype(nullptr) nullptr_t;\n"
        "    namespace __cxx11 {\n"
        "    } // namespace __cxx11\n"
        "} // namespace std\n"
        "namespace __gnu_cxx {\n"
        "    namespace __cxx11 {\n"
        "    } // namespace __cxx11\n"
        "} // namespace __gnu_cxx\n"
        "namespace NS1 {\n"
        "    template<class T> class A {\n"
        "        A<T>(std::size_t n);\n"
        "        const T * Data() const;\n"
        "        T * _data;\n"
        "    }; // class A\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    astCollection.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace std {\n"
        "    typedef unsigned long size_t;\n"
        "    typedef long ptrdiff_t;\n"
        "    typedef decltype(nullptr) nullptr_t;\n"
        "    namespace __cxx11 {\n"
        "    } // namespace __cxx11\n"
        "} // namespace std\n"
        "namespace __gnu_cxx {\n"
        "    namespace __cxx11 {\n"
        "    } // namespace __cxx11\n"
        "} // namespace __gnu_cxx\n"
        "namespace std {\n"
        "} // namespace std\n"
        "namespace NS1 {\n"
        "    template<class T> class A {\n"
        "        A<T>(std::size_t n);\n"
        "        const T * Data() const;\n"
        "        T * _data;\n"
        "    }; // class A\n"
        "} // namespace NS1\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

static OptionsList compileOptionsWPEFramework =
    {
        "-x",
        "c++",
        "-std=c++11",
        "-I.",
    };

TEST_FIXTURE(ParserTest, IMemory)
{
    Parser parser(TestData::IMemoryHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{2}, astCollection.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{4}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr nsCore = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, nsCore);
    EXPECT_EQ("Core", nsCore->Name());
    EXPECT_EQ(nullptr, nsCore->Parent());

    EXPECT_EQ(size_t{1}, nsCore->Namespaces().size());
    EXPECT_EQ(size_t{0}, nsCore->Classes().size());
    EXPECT_EQ(size_t{1}, nsCore->Structs().size());
    EXPECT_EQ(size_t{1}, nsCore->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsCore->Enums().size());
    EXPECT_EQ(size_t{0}, nsCore->Functions().size());
    EXPECT_EQ(size_t{0}, nsCore->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsCore->Variables().size());
    EXPECT_EQ(size_t{0}, nsCore->FunctionTemplates().size());

    const Namespace::Ptr nsWPEFramework = astCollection.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{3}, nsWPEFramework->Namespaces().size()); // PluginHost, Web, Exchange
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Functions().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Variables().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->FunctionTemplates().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Functions().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Variables().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->FunctionTemplates().size());

    const Namespace::Ptr nsWeb = nsWPEFramework->Namespaces()[1];
    ASSERT_NE(nullptr, nsWeb);
    EXPECT_EQ("Web", nsWeb->Name());
    EXPECT_EQ(nsWPEFramework, nsWeb->Parent());

    EXPECT_EQ(size_t{0}, nsWeb->Namespaces().size());
    EXPECT_EQ(size_t{2}, nsWeb->Classes().size());
    EXPECT_EQ(size_t{0}, nsWeb->Structs().size());
    EXPECT_EQ(size_t{0}, nsWeb->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsWeb->Enums().size());
    EXPECT_EQ(size_t{0}, nsWeb->Functions().size());
    EXPECT_EQ(size_t{0}, nsWeb->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsWeb->Variables().size());
    EXPECT_EQ(size_t{0}, nsWeb->FunctionTemplates().size());

    const Namespace::Ptr nsExchange = nsWPEFramework->Namespaces()[2];
    ASSERT_NE(nullptr, nsExchange);
    EXPECT_EQ("Exchange", nsExchange->Name());
    EXPECT_EQ(nsWPEFramework, nsExchange->Parent());

    EXPECT_EQ(size_t{0}, nsExchange->Namespaces().size());
    EXPECT_EQ(size_t{0}, nsExchange->Classes().size());
    EXPECT_EQ(size_t{1}, nsExchange->Structs().size());
    EXPECT_EQ(size_t{0}, nsExchange->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsExchange->Enums().size());
    EXPECT_EQ(size_t{0}, nsExchange->Functions().size());
    EXPECT_EQ(size_t{0}, nsExchange->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsExchange->Variables().size());
    EXPECT_EQ(size_t{0}, nsExchange->FunctionTemplates().size());

    const Struct::Ptr intf = nsExchange->Structs()[0];
    ASSERT_NE(nullptr, intf);
    EXPECT_EQ("IMemory", intf->Name());
    EXPECT_EQ(nsExchange, intf->Parent());

    std::string expected =
        "namespace Core {\n"
        "    struct IUnknown {\n"
        "    }; // struct IUnknown\n"
        "    template<class CONTEXT> class ProxyType {\n"
        "    }; // class ProxyType\n"
        "    namespace JSON {\n"
        "        struct IElement {\n"
        "        }; // struct IElement\n"
        "    } // namespace JSON\n"
        "} // namespace Core\n"
        "namespace WPEFramework {\n"
        "    namespace PluginHost {\n"
        "        struct IShell {\n"
        "        }; // struct IShell\n"
        "        class Channel {\n"
        "        }; // class Channel\n"
        "    } // namespace PluginHost\n"
        "    namespace Web {\n"
        "        class Request {\n"
        "        }; // class Request\n"
        "        class Response {\n"
        "        }; // class Response\n"
        "    } // namespace Web\n"
        "    namespace Exchange {\n"
        "        struct IMemory : virtual public Core::IUnknown {\n"
        "            enum {\n"
        "                ID = 82,\n"
        "            }; // enum <anonymous>\n"
        "            virtual ~IMemory();\n"
        "            virtual uint64 Resident() const = 0;\n"
        "            virtual uint64 Allocated() const = 0;\n"
        "            virtual uint64 Shared() const = 0;\n"
        "            virtual uint8 Processes() const = 0;\n"
        "            virtual const bool IsOperational() const = 0;\n"
        "        }; // struct IMemory\n"
        "    } // namespace Exchange\n"
        "} // namespace WPEFramework\n"
        "typedef unsigned long long uint64;\n"
        "typedef unsigned int uint32;\n"
        "typedef unsigned short uint16;\n"
        "typedef unsigned char uint8;\n"
        "class string {\n"
        "}; // class string\n";
    std::ostringstream stream;
    CodeGenerator codeGenerator(stream);
    astCollection.Visit(codeGenerator);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace Core {\n"
        "    struct IUnknown {\n"
        "    }; // struct IUnknown\n"
        "    template<class CONTEXT> class ProxyType {\n"
        "    }; // class ProxyType\n"
        "    namespace JSON {\n"
        "        struct IElement {\n"
        "        }; // struct IElement\n"
        "    } // namespace JSON\n"
        "} // namespace Core\n"
        "namespace WPEFramework {\n"
        "    namespace PluginHost {\n"
        "        struct IShell {\n"
        "        }; // struct IShell\n"
        "        class Channel {\n"
        "        }; // class Channel\n"
        "    } // namespace PluginHost\n"
        "    namespace Web {\n"
        "        class Request {\n"
        "        }; // class Request\n"
        "        class Response {\n"
        "        }; // class Response\n"
        "    } // namespace Web\n"
        "} // namespace WPEFramework\n"
        "typedef unsigned long long uint64;\n"
        "typedef unsigned int uint32;\n"
        "typedef unsigned short uint16;\n"
        "typedef unsigned char uint8;\n"
        "class string {\n"
        "}; // class string\n"
        "namespace WPEFramework {\n"
        "    namespace Exchange {\n"
        "        struct IMemory : virtual public Core::IUnknown {\n"
        "            enum {\n"
        "                ID = 82,\n"
        "            }; // enum <anonymous>\n"
        "            virtual ~IMemory();\n"
        "            virtual uint64 Resident() const = 0;\n"
        "            virtual uint64 Allocated() const = 0;\n"
        "            virtual uint64 Shared() const = 0;\n"
        "            virtual uint8 Processes() const = 0;\n"
        "            virtual const bool IsOperational() const = 0;\n"
        "        }; // struct IMemory\n"
        "    } // namespace Exchange\n"
        "} // namespace WPEFramework\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, IPluginComplete)
{
    Parser parser(TestData::IPluginHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{2}, astCollection.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{4}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr nsCore = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, nsCore);
    EXPECT_EQ("Core", nsCore->Name());
    EXPECT_EQ(nullptr, nsCore->Parent());

    EXPECT_EQ(size_t{1}, nsCore->Namespaces().size());
    EXPECT_EQ(size_t{0}, nsCore->Classes().size());
    EXPECT_EQ(size_t{1}, nsCore->Structs().size());
    EXPECT_EQ(size_t{1}, nsCore->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsCore->Enums().size());
    EXPECT_EQ(size_t{0}, nsCore->Functions().size());
    EXPECT_EQ(size_t{0}, nsCore->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsCore->Variables().size());
    EXPECT_EQ(size_t{0}, nsCore->FunctionTemplates().size());

    const Namespace::Ptr nsWPEFramework = astCollection.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{2}, nsWPEFramework->Namespaces().size()); // PluginHost, Web
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Functions().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Variables().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->FunctionTemplates().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{7}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Functions().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Variables().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->FunctionTemplates().size());

    const Namespace::Ptr nsWeb = nsWPEFramework->Namespaces()[1];
    ASSERT_NE(nullptr, nsWeb);
    EXPECT_EQ("Web", nsWeb->Name());
    EXPECT_EQ(nsWPEFramework, nsWeb->Parent());

    EXPECT_EQ(size_t{0}, nsWeb->Namespaces().size());
    EXPECT_EQ(size_t{2}, nsWeb->Classes().size());
    EXPECT_EQ(size_t{0}, nsWeb->Structs().size());
    EXPECT_EQ(size_t{0}, nsWeb->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsWeb->Enums().size());
    EXPECT_EQ(size_t{0}, nsWeb->Functions().size());
    EXPECT_EQ(size_t{0}, nsWeb->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsWeb->Variables().size());
    EXPECT_EQ(size_t{0}, nsWeb->FunctionTemplates().size());

    const Struct::Ptr IPlugin = nsPluginHost->Structs()[1];
    ASSERT_NE(nullptr, IPlugin);
    EXPECT_EQ("IPlugin", IPlugin->Name());
    EXPECT_EQ(nsPluginHost, IPlugin->Parent());

    std::string expected =
        "namespace Core {\n"
            "    struct IUnknown {\n"
            "    }; // struct IUnknown\n"
            "    template<class CONTEXT> class ProxyType {\n"
            "    }; // class ProxyType\n"
            "    namespace JSON {\n"
            "        struct IElement {\n"
            "        }; // struct IElement\n"
            "    } // namespace JSON\n"
            "} // namespace Core\n"
            "namespace WPEFramework {\n"
            "    namespace PluginHost {\n"
            "        struct IShell {\n"
            "        }; // struct IShell\n"
            "        class Channel {\n"
            "        }; // class Channel\n"
            "        struct IPlugin : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 32,\n"
            "            }; // enum <anonymous>\n"
            "            struct INotification : virtual public Core::IUnknown {\n"
            "                enum {\n"
            "                    ID = 33,\n"
            "                }; // enum <anonymous>\n"
            "                virtual ~INotification();\n"
            "                virtual void StateChange(PluginHost::IShell * plugin) = 0;\n"
            "            }; // struct INotification\n"
            "            virtual ~IPlugin();\n"
            "            virtual const string Initialize(PluginHost::IShell * shell) = 0;\n"
            "            virtual void Deinitialize(PluginHost::IShell * shell) = 0;\n"
            "            virtual string Information() const = 0;\n"
            "        }; // struct IPlugin\n"
            "        struct IPluginExtended : public struct WPEFramework::PluginHost::IPlugin {\n"
            "            enum {\n"
            "                ID = 34,\n"
            "            }; // enum <anonymous>\n"
            "            virtual ~IPluginExtended();\n"
            "            virtual bool Attach(PluginHost::Channel & channel) = 0;\n"
            "            virtual void Detach(PluginHost::Channel & channel) = 0;\n"
            "        }; // struct IPluginExtended\n"
            "        struct IWeb : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 35,\n"
            "            }; // enum <anonymous>\n"
            "            virtual void Inbound(Web::Request & request) = 0;\n"
            "            virtual Core::ProxyType<Web::Response> Process(const Web::Request & request) = 0;\n"
            "        }; // struct IWeb\n"
            "        struct IWebSocket : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 36,\n"
            "            }; // enum <anonymous>\n"
            "            virtual Core::ProxyType<Core::JSON::IElement> Inbound(const string & identifier) = 0;\n"
            "            virtual Core::ProxyType<Core::JSON::IElement> Inbound(const uint32 ID, const Core::JSON::IElement & element) = 0;\n"
            "        }; // struct IWebSocket\n"
            "        struct IChannel : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 37,\n"
            "            }; // enum <anonymous>\n"
            "            virtual uint32 Inbound(const uint32 ID, const uint8 [] data, const uint16 length) = 0;\n"
            "            virtual uint32 Outbound(const uint32 ID, uint8 [] data, const uint16 length) const = 0;\n"
            "        }; // struct IChannel\n"
            "        struct ISecurity : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 38,\n"
            "            }; // enum <anonymous>\n"
            "            virtual bool Allowed(const Web::Request & request) = 0;\n"
            "            virtual Core::ProxyType<Web::Response> Options(const Web::Request & request) = 0;\n"
            "        }; // struct ISecurity\n"
            "    } // namespace PluginHost\n"
            "    namespace Web {\n"
            "        class Request {\n"
            "        }; // class Request\n"
            "        class Response {\n"
            "        }; // class Response\n"
            "    } // namespace Web\n"
            "} // namespace WPEFramework\n"
            "typedef unsigned long long uint64;\n"
            "typedef unsigned int uint32;\n"
            "typedef unsigned short uint16;\n"
            "typedef unsigned char uint8;\n"
            "class string {\n"
            "}; // class string\n";
    std::ostringstream stream;
    CodeGenerator codeGenerator(stream);
    astCollection.Visit(codeGenerator);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace Core {\n"
            "    struct IUnknown {\n"
            "    }; // struct IUnknown\n"
            "    template<class CONTEXT> class ProxyType {\n"
            "    }; // class ProxyType\n"
            "    namespace JSON {\n"
            "        struct IElement {\n"
            "        }; // struct IElement\n"
            "    } // namespace JSON\n"
            "} // namespace Core\n"
            "namespace WPEFramework {\n"
            "    namespace PluginHost {\n"
            "        struct IShell {\n"
            "        }; // struct IShell\n"
            "        class Channel {\n"
            "        }; // class Channel\n"
            "    } // namespace PluginHost\n"
            "    namespace Web {\n"
            "        class Request {\n"
            "        }; // class Request\n"
            "        class Response {\n"
            "        }; // class Response\n"
            "    } // namespace Web\n"
            "} // namespace WPEFramework\n"
            "typedef unsigned long long uint64;\n"
            "typedef unsigned int uint32;\n"
            "typedef unsigned short uint16;\n"
            "typedef unsigned char uint8;\n"
            "class string {\n"
            "}; // class string\n"
            "namespace WPEFramework {\n"
            "    namespace Web {\n"
            "        class Request {\n"
            "        }; // class Request\n"
            "        class Response {\n"
            "        }; // class Response\n"
            "    } // namespace Web\n"
            "    namespace PluginHost {\n"
            "        struct IShell {\n"
            "        }; // struct IShell\n"
            "        class Channel {\n"
            "        }; // class Channel\n"
            "        struct IPlugin : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 32,\n"
            "            }; // enum <anonymous>\n"
            "            struct INotification : virtual public Core::IUnknown {\n"
            "                enum {\n"
            "                    ID = 33,\n"
            "                }; // enum <anonymous>\n"
            "                virtual ~INotification();\n"
            "                virtual void StateChange(PluginHost::IShell * plugin) = 0;\n"
            "            }; // struct INotification\n"
            "            virtual ~IPlugin();\n"
            "            virtual const string Initialize(PluginHost::IShell * shell) = 0;\n"
            "            virtual void Deinitialize(PluginHost::IShell * shell) = 0;\n"
            "            virtual string Information() const = 0;\n"
            "        }; // struct IPlugin\n"
            "        struct IPluginExtended : public struct WPEFramework::PluginHost::IPlugin {\n"
            "            enum {\n"
            "                ID = 34,\n"
            "            }; // enum <anonymous>\n"
            "            virtual ~IPluginExtended();\n"
            "            virtual bool Attach(PluginHost::Channel & channel) = 0;\n"
            "            virtual void Detach(PluginHost::Channel & channel) = 0;\n"
            "        }; // struct IPluginExtended\n"
            "        struct IWeb : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 35,\n"
            "            }; // enum <anonymous>\n"
            "            virtual void Inbound(Web::Request & request) = 0;\n"
            "            virtual Core::ProxyType<Web::Response> Process(const Web::Request & request) = 0;\n"
            "        }; // struct IWeb\n"
            "        struct IWebSocket : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 36,\n"
            "            }; // enum <anonymous>\n"
            "            virtual Core::ProxyType<Core::JSON::IElement> Inbound(const string & identifier) = 0;\n"
            "            virtual Core::ProxyType<Core::JSON::IElement> Inbound(const uint32 ID, const Core::JSON::IElement & element) = 0;\n"
            "        }; // struct IWebSocket\n"
            "        struct IChannel : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 37,\n"
            "            }; // enum <anonymous>\n"
            "            virtual uint32 Inbound(const uint32 ID, const uint8 [] data, const uint16 length) = 0;\n"
            "            virtual uint32 Outbound(const uint32 ID, uint8 [] data, const uint16 length) const = 0;\n"
            "        }; // struct IChannel\n"
            "        struct ISecurity : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 38,\n"
            "            }; // enum <anonymous>\n"
            "            virtual bool Allowed(const Web::Request & request) = 0;\n"
            "            virtual Core::ProxyType<Web::Response> Options(const Web::Request & request) = 0;\n"
            "        }; // struct ISecurity\n"
            "    } // namespace PluginHost\n"
            "} // namespace WPEFramework\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, IPlugin)
{
    Parser parser(TestData::IPluginHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{2}, astCollection.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{4}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr nsCore = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, nsCore);
    EXPECT_EQ("Core", nsCore->Name());
    EXPECT_EQ(nullptr, nsCore->Parent());

    EXPECT_EQ(size_t{1}, nsCore->Namespaces().size());
    EXPECT_EQ(size_t{0}, nsCore->Classes().size());
    EXPECT_EQ(size_t{1}, nsCore->Structs().size());
    EXPECT_EQ(size_t{1}, nsCore->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsCore->Enums().size());
    EXPECT_EQ(size_t{0}, nsCore->Functions().size());
    EXPECT_EQ(size_t{0}, nsCore->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsCore->Variables().size());
    EXPECT_EQ(size_t{0}, nsCore->FunctionTemplates().size());

    const Namespace::Ptr nsWPEFramework = astCollection.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{2}, nsWPEFramework->Namespaces().size()); // PluginHost, Web
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Functions().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Variables().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->FunctionTemplates().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{7}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Functions().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Variables().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->FunctionTemplates().size());

    const Namespace::Ptr nsWeb = nsWPEFramework->Namespaces()[1];
    ASSERT_NE(nullptr, nsWeb);
    EXPECT_EQ("Web", nsWeb->Name());
    EXPECT_EQ(nsWPEFramework, nsWeb->Parent());

    EXPECT_EQ(size_t{0}, nsWeb->Namespaces().size());
    EXPECT_EQ(size_t{2}, nsWeb->Classes().size());
    EXPECT_EQ(size_t{0}, nsWeb->Structs().size());
    EXPECT_EQ(size_t{0}, nsWeb->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsWeb->Enums().size());
    EXPECT_EQ(size_t{0}, nsWeb->Functions().size());
    EXPECT_EQ(size_t{0}, nsWeb->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsWeb->Variables().size());
    EXPECT_EQ(size_t{0}, nsWeb->FunctionTemplates().size());

    const Struct::Ptr IPlugin = nsPluginHost->Structs()[1];
    ASSERT_NE(nullptr, IPlugin);
    EXPECT_EQ("IPlugin", IPlugin->Name());
    EXPECT_EQ(nsPluginHost, IPlugin->Parent());

    std::string expected =
        "struct IPlugin : virtual public Core::IUnknown {\n"
        "    enum {\n"
        "        ID = 32,\n"
        "    }; // enum <anonymous>\n"
        "    struct INotification : virtual public Core::IUnknown {\n"
        "        enum {\n"
        "            ID = 33,\n"
        "        }; // enum <anonymous>\n"
        "        virtual ~INotification();\n"
        "        virtual void StateChange(PluginHost::IShell * plugin) = 0;\n"
        "    }; // struct INotification\n"
        "    virtual ~IPlugin();\n"
        "    virtual const string Initialize(PluginHost::IShell * shell) = 0;\n"
        "    virtual void Deinitialize(PluginHost::IShell * shell) = 0;\n"
        "    virtual string Information() const = 0;\n"
        "}; // struct IPlugin\n";
    std::ostringstream stream;
    CodeGenerator codeGenerator(stream);
    IPlugin->Visit(codeGenerator);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, IPluginExtended)
{
    Parser parser(TestData::IPluginHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{2}, astCollection.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{4}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr nsWPEFramework = astCollection.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{2}, nsWPEFramework->Namespaces().size()); // PluginHost, Web
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Functions().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Variables().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->FunctionTemplates().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{7}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Functions().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Variables().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->FunctionTemplates().size());

    const Struct::Ptr IPluginExtended = nsPluginHost->Structs()[2];
    ASSERT_NE(nullptr, IPluginExtended);
    EXPECT_EQ("IPluginExtended", IPluginExtended->Name());
    EXPECT_EQ(nsPluginHost, IPluginExtended->Parent());

    std::string expected =
        "struct IPluginExtended : public struct WPEFramework::PluginHost::IPlugin {\n"
            "    enum {\n"
            "        ID = 34,\n"
            "    }; // enum <anonymous>\n"
            "    virtual ~IPluginExtended();\n"
            "    virtual bool Attach(PluginHost::Channel & channel) = 0;\n"
            "    virtual void Detach(PluginHost::Channel & channel) = 0;\n"
            "}; // struct IPluginExtended\n";
    std::ostringstream stream;
    CodeGenerator codeGenerator(stream);
    IPluginExtended->Visit(codeGenerator);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

    expected =
        "namespace Core {\n"
            "    struct IUnknown {\n"
            "    }; // struct IUnknown\n"
            "    template<class CONTEXT> class ProxyType {\n"
            "    }; // class ProxyType\n"
            "    namespace JSON {\n"
            "        struct IElement {\n"
            "        }; // struct IElement\n"
            "    } // namespace JSON\n"
            "} // namespace Core\n"
            "namespace WPEFramework {\n"
            "    namespace PluginHost {\n"
            "        struct IShell {\n"
            "        }; // struct IShell\n"
            "        class Channel {\n"
            "        }; // class Channel\n"
            "    } // namespace PluginHost\n"
            "    namespace Web {\n"
            "        class Request {\n"
            "        }; // class Request\n"
            "        class Response {\n"
            "        }; // class Response\n"
            "    } // namespace Web\n"
            "} // namespace WPEFramework\n"
            "typedef unsigned long long uint64;\n"
            "typedef unsigned int uint32;\n"
            "typedef unsigned short uint16;\n"
            "typedef unsigned char uint8;\n"
            "class string {\n"
            "}; // class string\n"
            "namespace WPEFramework {\n"
            "    namespace Web {\n"
            "        class Request {\n"
            "        }; // class Request\n"
            "        class Response {\n"
            "        }; // class Response\n"
            "    } // namespace Web\n"
            "    namespace PluginHost {\n"
            "        struct IShell {\n"
            "        }; // struct IShell\n"
            "        class Channel {\n"
            "        }; // class Channel\n"
            "        struct IPlugin : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 32,\n"
            "            }; // enum <anonymous>\n"
            "            struct INotification : virtual public Core::IUnknown {\n"
            "                enum {\n"
            "                    ID = 33,\n"
            "                }; // enum <anonymous>\n"
            "                virtual ~INotification();\n"
            "                virtual void StateChange(PluginHost::IShell * plugin) = 0;\n"
            "            }; // struct INotification\n"
            "            virtual ~IPlugin();\n"
            "            virtual const string Initialize(PluginHost::IShell * shell) = 0;\n"
            "            virtual void Deinitialize(PluginHost::IShell * shell) = 0;\n"
            "            virtual string Information() const = 0;\n"
            "        }; // struct IPlugin\n"
            "        struct IPluginExtended : public struct WPEFramework::PluginHost::IPlugin {\n"
            "            enum {\n"
            "                ID = 34,\n"
            "            }; // enum <anonymous>\n"
            "            virtual ~IPluginExtended();\n"
            "            virtual bool Attach(PluginHost::Channel & channel) = 0;\n"
            "            virtual void Detach(PluginHost::Channel & channel) = 0;\n"
            "        }; // struct IPluginExtended\n"
            "        struct IWeb : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 35,\n"
            "            }; // enum <anonymous>\n"
            "            virtual void Inbound(Web::Request & request) = 0;\n"
            "            virtual Core::ProxyType<Web::Response> Process(const Web::Request & request) = 0;\n"
            "        }; // struct IWeb\n"
            "        struct IWebSocket : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 36,\n"
            "            }; // enum <anonymous>\n"
            "            virtual Core::ProxyType<Core::JSON::IElement> Inbound(const string & identifier) = 0;\n"
            "            virtual Core::ProxyType<Core::JSON::IElement> Inbound(const uint32 ID, const Core::JSON::IElement & element) = 0;\n"
            "        }; // struct IWebSocket\n"
            "        struct IChannel : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 37,\n"
            "            }; // enum <anonymous>\n"
            "            virtual uint32 Inbound(const uint32 ID, const uint8 [] data, const uint16 length) = 0;\n"
            "            virtual uint32 Outbound(const uint32 ID, uint8 [] data, const uint16 length) const = 0;\n"
            "        }; // struct IChannel\n"
            "        struct ISecurity : virtual public Core::IUnknown {\n"
            "            enum {\n"
            "                ID = 38,\n"
            "            }; // enum <anonymous>\n"
            "            virtual bool Allowed(const Web::Request & request) = 0;\n"
            "            virtual Core::ProxyType<Web::Response> Options(const Web::Request & request) = 0;\n"
            "        }; // struct ISecurity\n"
            "    } // namespace PluginHost\n"
            "} // namespace WPEFramework\n";
    stream.str("");
    parser.TraverseTree(stream);
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, IWeb)
{
    Parser parser(TestData::IPluginHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{2}, astCollection.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{4}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr nsWPEFramework = astCollection.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{2}, nsWPEFramework->Namespaces().size()); // PluginHost, Web
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Functions().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Variables().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->FunctionTemplates().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{7}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Functions().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Variables().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->FunctionTemplates().size());

    const Struct::Ptr IWeb = nsPluginHost->Structs()[3];
    ASSERT_NE(nullptr, IWeb);
    EXPECT_EQ("IWeb", IWeb->Name());
    EXPECT_EQ(nsPluginHost, IWeb->Parent());

    std::string expected =
        "struct IWeb : virtual public Core::IUnknown {\n"
            "    enum {\n"
            "        ID = 35,\n"
            "    }; // enum <anonymous>\n"
            "    virtual void Inbound(Web::Request & request) = 0;\n"
            "    virtual Core::ProxyType<Web::Response> Process(const Web::Request & request) = 0;\n"
            "}; // struct IWeb\n";
    std::ostringstream stream;
    CodeGenerator codeGenerator(stream);
    IWeb->Visit(codeGenerator);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, IWebSocket)
{
    Parser parser(TestData::IPluginHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{2}, astCollection.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{4}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr nsCore = astCollection.Namespaces()[0];
    ASSERT_NE(nullptr, nsCore);
    EXPECT_EQ("Core", nsCore->Name());
    EXPECT_EQ(nullptr, nsCore->Parent());

    EXPECT_EQ(size_t{1}, nsCore->Namespaces().size());
    EXPECT_EQ(size_t{0}, nsCore->Classes().size());
    EXPECT_EQ(size_t{1}, nsCore->Structs().size());
    EXPECT_EQ(size_t{1}, nsCore->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsCore->Enums().size());
    EXPECT_EQ(size_t{0}, nsCore->Functions().size());
    EXPECT_EQ(size_t{0}, nsCore->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsCore->Variables().size());
    EXPECT_EQ(size_t{0}, nsCore->FunctionTemplates().size());

    const Namespace::Ptr nsWPEFramework = astCollection.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{2}, nsWPEFramework->Namespaces().size()); // PluginHost, Web
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Functions().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Variables().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->FunctionTemplates().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{7}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Functions().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Variables().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->FunctionTemplates().size());

    const Namespace::Ptr nsWeb = nsWPEFramework->Namespaces()[1];
    ASSERT_NE(nullptr, nsWeb);
    EXPECT_EQ("Web", nsWeb->Name());
    EXPECT_EQ(nsWPEFramework, nsWeb->Parent());

    EXPECT_EQ(size_t{0}, nsWeb->Namespaces().size());
    EXPECT_EQ(size_t{2}, nsWeb->Classes().size());
    EXPECT_EQ(size_t{0}, nsWeb->Structs().size());
    EXPECT_EQ(size_t{0}, nsWeb->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsWeb->Enums().size());
    EXPECT_EQ(size_t{0}, nsWeb->Functions().size());
    EXPECT_EQ(size_t{0}, nsWeb->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsWeb->Variables().size());
    EXPECT_EQ(size_t{0}, nsWeb->FunctionTemplates().size());

    const Struct::Ptr IWebSocket = nsPluginHost->Structs()[4];
    ASSERT_NE(nullptr, IWebSocket);
    EXPECT_EQ("IWebSocket", IWebSocket->Name());
    EXPECT_EQ(nsPluginHost, IWebSocket->Parent());

    std::string expected =
        "struct IWebSocket : virtual public Core::IUnknown {\n"
            "    enum {\n"
            "        ID = 36,\n"
            "    }; // enum <anonymous>\n"
            "    virtual Core::ProxyType<Core::JSON::IElement> Inbound(const string & identifier) = 0;\n"
            "    virtual Core::ProxyType<Core::JSON::IElement> Inbound(const uint32 ID, const Core::JSON::IElement & element) = 0;\n"
            "}; // struct IWebSocket\n";
    std::ostringstream stream;
    CodeGenerator codeGenerator(stream);
    IWebSocket->Visit(codeGenerator);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, IChannel)
{
    Parser parser(TestData::IPluginHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{2}, astCollection.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{4}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr nsWPEFramework = astCollection.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{2}, nsWPEFramework->Namespaces().size()); // PluginHost, Web
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Functions().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Variables().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->FunctionTemplates().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{7}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Functions().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Variables().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->FunctionTemplates().size());

    const Struct::Ptr IChannel = nsPluginHost->Structs()[5];
    ASSERT_NE(nullptr, IChannel);
    EXPECT_EQ("IChannel", IChannel->Name());
    EXPECT_EQ(nsPluginHost, IChannel->Parent());

    std::string expected =
        "struct IChannel : virtual public Core::IUnknown {\n"
            "    enum {\n"
            "        ID = 37,\n"
            "    }; // enum <anonymous>\n"
            "    virtual uint32 Inbound(const uint32 ID, const uint8 [] data, const uint16 length) = 0;\n"
            "    virtual uint32 Outbound(const uint32 ID, uint8 [] data, const uint16 length) const = 0;\n"
            "}; // struct IChannel\n";
    std::ostringstream stream;
    CodeGenerator codeGenerator(stream);
    IChannel->Visit(codeGenerator);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ParserTest, ISecurity)
{
    Parser parser(TestData::IPluginHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const ASTCollection & astCollection = parser.GetASTCollection();
    EXPECT_EQ(nullptr, astCollection.Parent());

    EXPECT_EQ(size_t{2}, astCollection.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, astCollection.Classes().size());
    EXPECT_EQ(size_t{0}, astCollection.Structs().size());
    EXPECT_EQ(size_t{0}, astCollection.ClassTemplates().size());
    EXPECT_EQ(size_t{0}, astCollection.Enums().size());
    EXPECT_EQ(size_t{0}, astCollection.Functions().size());
    EXPECT_EQ(size_t{4}, astCollection.Typedefs().size());
    EXPECT_EQ(size_t{0}, astCollection.Variables().size());
    EXPECT_EQ(size_t{0}, astCollection.FunctionTemplates().size());

    const Namespace::Ptr nsWPEFramework = astCollection.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{2}, nsWPEFramework->Namespaces().size()); // PluginHost, Web
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Functions().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Variables().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->FunctionTemplates().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{7}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->ClassTemplates().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Functions().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Typedefs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Variables().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->FunctionTemplates().size());

    const Struct::Ptr ISecurity = nsPluginHost->Structs()[6];
    ASSERT_NE(nullptr, ISecurity);
    EXPECT_EQ("ISecurity", ISecurity->Name());
    EXPECT_EQ(nsPluginHost, ISecurity->Parent());

    std::string expected =
        "struct ISecurity : virtual public Core::IUnknown {\n"
            "    enum {\n"
            "        ID = 38,\n"
            "    }; // enum <anonymous>\n"
            "    virtual bool Allowed(const Web::Request & request) = 0;\n"
            "    virtual Core::ProxyType<Web::Response> Options(const Web::Request & request) = 0;\n"
            "}; // struct ISecurity\n";
    std::ostringstream stream;
    CodeGenerator codeGenerator(stream);
    ISecurity->Visit(codeGenerator);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

} // namespace Test
} // namespace CPPParser
