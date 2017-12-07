#include "include/Container.h"

#include <fstream>
#include <iomanip>
#include "include/Namespace.h"
#include "include/Class.h"
#include "include/Struct.h"
#include "include/Enum.h"
#include "include/Typedef.h"

using namespace std;
using namespace Utility;

namespace CPPParser
{

void Container::ShowContents(std::ostream & stream, int indent) const
{
    for (auto const & aNamespace : _namespaces)
    {
        aNamespace->Show(stream, indent);
    }
    for (auto const & aClass : _classes)
    {
        aClass->Show(stream, indent);
    }
    for (auto const & aStruct : _structs)
    {
        aStruct->Show(stream, indent);
    }
    for (auto const & aEnum : _enums)
    {
        aEnum->Show(stream, indent);
    }
    for (auto const & aFunction : _functions)
    {
        aFunction->Show(stream, indent);
    }
}

void Container::GenerateCodeContents(std::ostream & stream, int indent) const
{
    for (auto const & item : _contents)
        item->GenerateCode(stream, indent);
}

void Container::Add(const std::shared_ptr<Declaration> & value)
{
    _contents.push_back(value);
    Namespace::Ptr aNamespace = dynamic_pointer_cast<Namespace>(value);
    if (aNamespace != nullptr)
    {
        AddNamespace(aNamespace);
        return;
    }
    Class::Ptr aClass = dynamic_pointer_cast<Class>(value);
    if (aClass != nullptr)
    {
        AddClass(aClass);
        return;
    }
    Struct::Ptr aStruct = dynamic_pointer_cast<Struct>(value);
    if (aStruct != nullptr)
    {
        AddStruct(aStruct);
        return;
    }
    Enum::Ptr aEnum = dynamic_pointer_cast<Enum>(value);
    if (aEnum != nullptr)
    {
        AddEnum(aEnum);
        return;
    }
    Function::Ptr aFunction = dynamic_pointer_cast<Function>(value);
    if (aFunction != nullptr)
    {
        AddFunction(aFunction);
        return;
    }
    Typedef::Ptr aTypedef = dynamic_pointer_cast<Typedef>(value);
    if (aTypedef != nullptr)
    {
        AddTypedef(aTypedef);
        return;
    }
}

void Container::AddNamespace(const Namespace::Ptr & value)
{
    _namespaces.push_back(value);
}

void Container::AddClass(const std::shared_ptr<Class> & value)
{
    _classes.push_back(value);
}

void Container::AddStruct(const std::shared_ptr<Struct> & value)
{
    _structs.push_back(value);
}

void Container::AddEnum(const std::shared_ptr<Enum> & value)
{
    _enums.push_back(value);
}

void Container::AddFunction(const std::shared_ptr<Function> & value)
{
    _functions.push_back(value);
}

void Container::AddTypedef(const std::shared_ptr<Typedef> & value)
{
    _typedefs.push_back(value);
}

template<class T>
bool FindElement(std::vector<std::shared_ptr<T>> & list, const std::string & name, std::shared_ptr<T> & result)
{
    result = {};
    for (auto const & item : list)
    {
        if (item->Name() == name)
        {
            result = item;
            return true;
        }
    }
    return false;
}
bool Container::FindNamespace(const std::string & name, std::shared_ptr<Namespace> & result)
{
    return FindElement(_namespaces, name, result);
}

bool Container::FindClass(const std::string & name, std::shared_ptr<Class> & result)
{
    return FindElement(_classes, name, result);
}

bool Container::FindStruct(const std::string & name, std::shared_ptr<Struct> & result)
{
    return FindElement(_structs, name, result);
}

bool Container::FindEnum(const std::string & name, std::shared_ptr<Enum> & result)
{
    return FindElement(_enums, name, result);
}

bool Container::FindFunction(const std::string & name, std::shared_ptr<Function> & result)
{
    return FindElement(_functions, name, result);
}

bool Container::FindTypedef(const std::string & name, std::shared_ptr<Typedef> & result)
{
    return FindElement(_typedefs, name, result);
}

} // namespace CPPParser
