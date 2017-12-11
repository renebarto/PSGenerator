#include "include/Object.h"

#include <fstream>
#include <iomanip>

using namespace std;
using namespace Utility;

namespace CPPParser
{

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

void Object::SetAccessSpecifier(AccessSpecifier accessSpecifier)
{
    _currentAccessSpecifier = accessSpecifier;
}

} // namespace CPPParser
