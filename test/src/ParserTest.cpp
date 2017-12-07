#include <gtest/gtest.h>
#include <include/Parser.h>
#include <include/TestData.h>

namespace CPPParser {
namespace Test {

class ParserTest : public ::testing::Test {
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

TEST_F(ParserTest, Empty)
{
    Parser parser(TestData::EmptyHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{0}, ast.Namespaces().size());
    EXPECT_EQ(size_t{0}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    std::string expected =
        "";
    std::ostringstream stream;
    ast.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, SingleNamespace)
{
    Parser parser(TestData::SingleNamespaceHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{1}, ast.Namespaces().size());
    EXPECT_EQ(size_t{0}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr ns = ast.Namespaces()[0];
    ASSERT_NE(nullptr, ns);
    EXPECT_EQ("NS1", ns->Name());
    EXPECT_EQ(nullptr, ns->Parent());

    EXPECT_EQ(size_t{0}, ns->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns->Classes().size());
    EXPECT_EQ(size_t{0}, ns->Structs().size());
    EXPECT_EQ(size_t{0}, ns->Enums().size());

    std::string expected =
        "namespace NS1 {\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    ast.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, NestedNamespace)
{
    Parser parser(TestData::NestedNamespaceHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{1}, ast.Namespaces().size());
    EXPECT_EQ(size_t{0}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr ns1 = ast.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("NS2", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns2->Classes().size());
    EXPECT_EQ(size_t{0}, ns2->Structs().size());
    EXPECT_EQ(size_t{0}, ns2->Enums().size());

    std::string expected =
        "namespace NS1 {\n"
        "    namespace NS2 {\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    ast.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, AnonymousNamespace)
{
    Parser parser(TestData::AnonymousNamespaceHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{1}, ast.Namespaces().size());
    EXPECT_EQ(size_t{0}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr ns1 = ast.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns2->Classes().size());
    EXPECT_EQ(size_t{0}, ns2->Structs().size());
    EXPECT_EQ(size_t{0}, ns2->Enums().size());

    std::string expected =
        "namespace NS1 {\n"
        "    namespace {\n"
        "    } // namespace <anonymous>\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    ast.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, Class)
{
    Parser parser(TestData::ClassHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{1}, ast.Namespaces().size());
    EXPECT_EQ(size_t{0}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr ns1 = ast.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{1}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());
    EXPECT_EQ(size_t{0}, ns1->Functions().size());

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

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("NS2", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{1}, ns2->Classes().size());
    EXPECT_EQ(size_t{0}, ns2->Structs().size());
    EXPECT_EQ(size_t{0}, ns2->Enums().size());
    EXPECT_EQ(size_t{0}, ns2->Functions().size());

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
        "            NS1::interface * X() const;\n"
        "        }; // class c\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    ast.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, Struct)
{
    Parser parser(TestData::StructHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{1}, ast.Namespaces().size());
    EXPECT_EQ(size_t{0}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr ns1 = ast.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());
    EXPECT_EQ(size_t{0}, ns1->Functions().size());

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("NS2", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns2->Classes().size());
    EXPECT_EQ(size_t{2}, ns2->Structs().size());
    EXPECT_EQ(size_t{0}, ns2->Enums().size());
    EXPECT_EQ(size_t{0}, ns2->Functions().size());

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
        "            NS1::NS2::interface * X() const;\n"
        "        }; // struct s\n"
        "    } // namespace NS2\n"
        "} // namespace NS1\n";
    std::ostringstream stream;
    ast.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, Enum)
{
    Parser parser(TestData::EnumHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{1}, ast.Namespaces().size());
    EXPECT_EQ(size_t{0}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr ns1 = ast.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("NS2", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns2->Classes().size());
    EXPECT_EQ(size_t{0}, ns2->Structs().size());
    EXPECT_EQ(size_t{1}, ns2->Enums().size());

    const Enum::Ptr enumeration = ns2->Enums()[0];
    ASSERT_NE(nullptr, enumeration);
    EXPECT_EQ("e", enumeration->Name());
    EXPECT_EQ(ns2, enumeration->Parent());

    std::string expected =
        "enum e : short {\n"
        "    a = 0,\n"
        "    b = 1,\n"
        "}; // enum e\n";
    std::ostringstream stream;
    enumeration->GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, EnumAnonymous)
{
    Parser parser(TestData::EnumAnonymousHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{1}, ast.Namespaces().size());
    EXPECT_EQ(size_t{0}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr ns1 = ast.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("NS2", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns2->Classes().size());
    EXPECT_EQ(size_t{0}, ns2->Structs().size());
    EXPECT_EQ(size_t{1}, ns2->Enums().size());

    const Enum::Ptr enumeration = ns2->Enums()[0];
    ASSERT_NE(nullptr, enumeration);
    EXPECT_EQ("", enumeration->Name());
    EXPECT_EQ(ns2, enumeration->Parent());

    std::string expected =
        "enum {\n"
        "    a = 0,\n"
        "    b = 1,\n"
        "}; // enum <anonymous>\n";
    std::ostringstream stream;
    enumeration->GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, Inheritance)
{
    Parser parser(TestData::InheritanceHeader());

    ASSERT_TRUE(parser.Parse(compileOptions));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{1}, ast.Namespaces().size());
    EXPECT_EQ(size_t{0}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr ns1 = ast.Namespaces()[0];
    ASSERT_NE(nullptr, ns1);
    EXPECT_EQ("NS1", ns1->Name());
    EXPECT_EQ(nullptr, ns1->Parent());

    EXPECT_EQ(size_t{1}, ns1->Namespaces().size());
    EXPECT_EQ(size_t{0}, ns1->Classes().size());
    EXPECT_EQ(size_t{0}, ns1->Structs().size());
    EXPECT_EQ(size_t{0}, ns1->Enums().size());

    const Namespace::Ptr ns2 = ns1->Namespaces()[0];
    ASSERT_NE(nullptr, ns2);
    EXPECT_EQ("NS2", ns2->Name());
    EXPECT_EQ(ns1, ns2->Parent());

    EXPECT_EQ(size_t{0}, ns2->Namespaces().size());
    EXPECT_EQ(size_t{2}, ns2->Classes().size());
    EXPECT_EQ(size_t{0}, ns2->Structs().size());
    EXPECT_EQ(size_t{0}, ns2->Enums().size());

    const Class::Ptr A = ns2->Classes()[0];
    ASSERT_NE(nullptr, A);
    EXPECT_EQ("A", A->Name());
    EXPECT_EQ(ns2, A->Parent());

    const Class::Ptr B = ns2->Classes()[1];
    ASSERT_NE(nullptr, B);
    EXPECT_EQ("B", B->Name());
    EXPECT_EQ(ns2, B->Parent());

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
    ast.GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

static OptionsList compileOptionsWPEFramework =
    {
        "-x",
        "c++",
        "-std=c++11",
        "-I.",
    };

TEST_F(ParserTest, IMemory)
{
    Parser parser(TestData::IMemoryHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{2}, ast.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr nsCore = ast.Namespaces()[0];
    ASSERT_NE(nullptr, nsCore);
    EXPECT_EQ("Core", nsCore->Name());
    EXPECT_EQ(nullptr, nsCore->Parent());

    EXPECT_EQ(size_t{1}, nsCore->Namespaces().size());
    EXPECT_EQ(size_t{0}, nsCore->Classes().size());
    EXPECT_EQ(size_t{1}, nsCore->Structs().size());
    EXPECT_EQ(size_t{0}, nsCore->Enums().size());

    const Namespace::Ptr nsWPEFramework = ast.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{3}, nsWPEFramework->Namespaces().size()); // PluginHost, Web, Exchange
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());

    const Namespace::Ptr nsWeb = nsWPEFramework->Namespaces()[1];
    ASSERT_NE(nullptr, nsWeb);
    EXPECT_EQ("Web", nsWeb->Name());
    EXPECT_EQ(nsWPEFramework, nsWeb->Parent());

    EXPECT_EQ(size_t{0}, nsWeb->Namespaces().size());
    EXPECT_EQ(size_t{2}, nsWeb->Classes().size());
    EXPECT_EQ(size_t{0}, nsWeb->Structs().size());
    EXPECT_EQ(size_t{0}, nsWeb->Enums().size());

    const Namespace::Ptr nsExchange = nsWPEFramework->Namespaces()[2];
    ASSERT_NE(nullptr, nsExchange);
    EXPECT_EQ("Exchange", nsExchange->Name());
    EXPECT_EQ(nsWPEFramework, nsExchange->Parent());

    EXPECT_EQ(size_t{0}, nsExchange->Namespaces().size());
    EXPECT_EQ(size_t{0}, nsExchange->Classes().size());
    EXPECT_EQ(size_t{1}, nsExchange->Structs().size());
    EXPECT_EQ(size_t{0}, nsExchange->Enums().size());

    const Struct::Ptr intf = nsExchange->Structs()[0];
    ASSERT_NE(nullptr, intf);
    EXPECT_EQ("IMemory", intf->Name());
    EXPECT_EQ(nsExchange, intf->Parent());

    std::string expected =
        "struct IMemory : virtual public Core::IUnknown {\n"
            "    enum {\n"
            "        ID = 82,\n"
            "    }; // enum <anonymous>\n"
            "    virtual ~IMemory();\n"
            "    virtual uint64 Resident() const = 0;\n"
            "    virtual uint64 Allocated() const = 0;\n"
            "    virtual uint64 Shared() const = 0;\n"
            "    virtual uint8 Processes() const = 0;\n"
            "    virtual const bool IsOperational() const = 0;\n"
            "}; // struct IMemory\n";
    std::ostringstream stream;
    intf->GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, IPlugin)
{
    Parser parser(TestData::IPluginHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{2}, ast.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr nsCore = ast.Namespaces()[0];
    ASSERT_NE(nullptr, nsCore);
    EXPECT_EQ("Core", nsCore->Name());
    EXPECT_EQ(nullptr, nsCore->Parent());

    EXPECT_EQ(size_t{1}, nsCore->Namespaces().size());
    EXPECT_EQ(size_t{0}, nsCore->Classes().size());
    EXPECT_EQ(size_t{1}, nsCore->Structs().size());
    EXPECT_EQ(size_t{0}, nsCore->Enums().size());

    const Namespace::Ptr nsWPEFramework = ast.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{2}, nsWPEFramework->Namespaces().size()); // PluginHost, Web
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{7}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());

    const Namespace::Ptr nsWeb = nsWPEFramework->Namespaces()[1];
    ASSERT_NE(nullptr, nsWeb);
    EXPECT_EQ("Web", nsWeb->Name());
    EXPECT_EQ(nsWPEFramework, nsWeb->Parent());

    EXPECT_EQ(size_t{0}, nsWeb->Namespaces().size());
    EXPECT_EQ(size_t{2}, nsWeb->Classes().size());
    EXPECT_EQ(size_t{0}, nsWeb->Structs().size());
    EXPECT_EQ(size_t{0}, nsWeb->Enums().size());

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
    IPlugin->GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, IPluginExtended)
{
    Parser parser(TestData::IPluginHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{2}, ast.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr nsWPEFramework = ast.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{2}, nsWPEFramework->Namespaces().size()); // PluginHost, Web
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{7}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());

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
    IPluginExtended->GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, IWeb)
{
    Parser parser(TestData::IPluginHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{2}, ast.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr nsWPEFramework = ast.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{2}, nsWPEFramework->Namespaces().size()); // PluginHost, Web
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{7}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());

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
    IWeb->GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, IWebSocket)
{
    Parser parser(TestData::IPluginHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{2}, ast.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr nsCore = ast.Namespaces()[0];
    ASSERT_NE(nullptr, nsCore);
    EXPECT_EQ("Core", nsCore->Name());
    EXPECT_EQ(nullptr, nsCore->Parent());

    EXPECT_EQ(size_t{1}, nsCore->Namespaces().size());
    EXPECT_EQ(size_t{0}, nsCore->Classes().size());
    EXPECT_EQ(size_t{1}, nsCore->Structs().size());
    EXPECT_EQ(size_t{0}, nsCore->Enums().size());

    const Namespace::Ptr nsWPEFramework = ast.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{2}, nsWPEFramework->Namespaces().size()); // PluginHost, Web
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{7}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());

    const Namespace::Ptr nsWeb = nsWPEFramework->Namespaces()[1];
    ASSERT_NE(nullptr, nsWeb);
    EXPECT_EQ("Web", nsWeb->Name());
    EXPECT_EQ(nsWPEFramework, nsWeb->Parent());

    EXPECT_EQ(size_t{0}, nsWeb->Namespaces().size());
    EXPECT_EQ(size_t{2}, nsWeb->Classes().size());
    EXPECT_EQ(size_t{0}, nsWeb->Structs().size());
    EXPECT_EQ(size_t{0}, nsWeb->Enums().size());

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
    IWebSocket->GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, IChannel)
{
    Parser parser(TestData::IPluginHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{2}, ast.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr nsWPEFramework = ast.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{2}, nsWPEFramework->Namespaces().size()); // PluginHost, Web
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{7}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());

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
    IChannel->GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_F(ParserTest, ISecurity)
{
    Parser parser(TestData::IPluginHeader());

    ASSERT_TRUE(parser.Parse(compileOptionsWPEFramework));

    const AST & ast = parser.GetAST();
    EXPECT_EQ(nullptr, ast.Parent());

    EXPECT_EQ(size_t{2}, ast.Namespaces().size()); // Core, WPEFramework
    EXPECT_EQ(size_t{1}, ast.Classes().size());
    EXPECT_EQ(size_t{0}, ast.Structs().size());
    EXPECT_EQ(size_t{0}, ast.Enums().size());

    const Namespace::Ptr nsWPEFramework = ast.Namespaces()[1];
    ASSERT_NE(nullptr, nsWPEFramework);
    EXPECT_EQ("WPEFramework", nsWPEFramework->Name());
    EXPECT_EQ(nullptr, nsWPEFramework->Parent());

    EXPECT_EQ(size_t{2}, nsWPEFramework->Namespaces().size()); // PluginHost, Web
    EXPECT_EQ(size_t{0}, nsWPEFramework->Classes().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Structs().size());
    EXPECT_EQ(size_t{0}, nsWPEFramework->Enums().size());

    const Namespace::Ptr nsPluginHost = nsWPEFramework->Namespaces()[0];
    ASSERT_NE(nullptr, nsPluginHost);
    EXPECT_EQ("PluginHost", nsPluginHost->Name());
    EXPECT_EQ(nsWPEFramework, nsPluginHost->Parent());

    EXPECT_EQ(size_t{0}, nsPluginHost->Namespaces().size());
    EXPECT_EQ(size_t{1}, nsPluginHost->Classes().size());
    EXPECT_EQ(size_t{7}, nsPluginHost->Structs().size());
    EXPECT_EQ(size_t{0}, nsPluginHost->Enums().size());

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
    ISecurity->GenerateCode(stream, 0);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

} // namespace Test
} // namespace CPPParser
