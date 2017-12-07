#include "include/Object.h"

#include <fstream>
#include <iomanip>

using namespace std;
using namespace Utility;

namespace CPPParser
{

void Object::ShowContents(std::ostream & stream, int indent) const
{
    Container::ShowContents(stream, indent);
    for (auto const & aBaseType : _baseTypes)
    {
        aBaseType->Show(stream, indent + 1);
    }
    for (auto const & aConstructor : _constructors)
    {
        aConstructor->Show(stream, indent + 1);
    }
    for (auto const & aDestructor : _destructors)
    {
        aDestructor->Show(stream, indent + 1);
    }
    for (auto const & aMethod : _methods)
    {
        aMethod->Show(stream, indent + 1);
    }
}

void Object::GenerateCodeContents(std::ostream & stream, int indent) const
{
    Container::GenerateCodeContents(stream, indent + 1);
}

void Object::Add(const Declaration::Ptr & value)
{
    Container::Add(value);
    Constructor::Ptr aConstructor = dynamic_pointer_cast<Constructor>(value);
    if (aConstructor != nullptr)
    {
        AddConstructor(aConstructor);
        return;
    }
    Destructor::Ptr aDestructor = dynamic_pointer_cast<Destructor>(value);
    if (aDestructor != nullptr)
    {
        AddDestructor(aDestructor);
        return;
    }
    Method::Ptr aMethod = dynamic_pointer_cast<Method>(value);
    if (aMethod != nullptr)
    {
        AddMethod(aMethod);
        return;
    }
}

void Object::AddConstructor(const Constructor::Ptr & value)
{
    _constructors.push_back(value);
}

void Object::AddDestructor(const Destructor::Ptr & value)
{
    _destructors.push_back(value);
}

void Object::AddMethod(const Method::Ptr & value)
{
    _methods.push_back(value);
}

void Object::AddBase(const Inheritance::Ptr & value)
{
    _baseTypes.push_back(value);
}

} // namespace CPPParser
