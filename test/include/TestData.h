#pragma once

namespace CPPParser {
namespace Test {
namespace TestData {

inline char PathSeparator()
{
    return '/';
}

inline std::string CombinePath(const std::string & basePath, const std::string & subPath)
{
    std::string result = basePath;
    if (result.empty())
        result = ".";
    if (result[result.length() - 1] != PathSeparator())
        result += PathSeparator();
    result += subPath;
    return result;
}
inline std::string TestRoot()
{
    return TESTDATA_ROOT;
}

inline std::string EmptyHeader() { return CombinePath(TestRoot(), "Empty.h"); }
inline std::string SingleNamespaceHeader() { return CombinePath(TestRoot(), "SingleNamespace.h"); }
inline std::string NestedNamespaceHeader() { return CombinePath(TestRoot(), "NestedNamespace.h"); }
inline std::string NamespaceWithVarsAndFunctionsHeader() { return CombinePath(TestRoot(), "NamespaceWithVarsAndFunctions.h"); }
inline std::string AnonymousNamespaceHeader() { return CombinePath(TestRoot(), "AnonymousNamespace.h"); }
inline std::string ClassHeader() { return CombinePath(TestRoot(), "Class.h"); }
inline std::string StructHeader() { return CombinePath(TestRoot(), "Struct.h"); }
inline std::string EnumHeader() { return CombinePath(TestRoot(), "Enum.h"); }
inline std::string EnumAnonymousHeader() { return CombinePath(TestRoot(), "EnumAnonymous.h"); }
inline std::string InheritanceHeader() { return CombinePath(TestRoot(), "Inheritance.h"); }
inline std::string TemplateFunctionHeader() { return CombinePath(TestRoot(), "TemplateFunction.h"); }
inline std::string TemplateClassHeader() { return CombinePath(TestRoot(), "TemplateClass.h"); }
inline std::string IMemoryHeader() { return CombinePath(TestRoot(), "IMemory.hpp"); }
inline std::string IPluginHeader() { return CombinePath(TestRoot(), "IPlugin.h"); }

} // namespace TestData
} // namespace Test
} // namespace CPPParser
