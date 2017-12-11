#pragma once

#include <clang-c/Index.h>
#include "include/Container.h"
#include "include/Function.h"
#include "include/Inheritance.h"

namespace CPPParser
{

class Object
    : public Container
{
public:
    using Ptr = std::shared_ptr<Object>;
    using List = std::vector<Ptr>;

    Object() = delete;
    explicit Object(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier,
                    AccessSpecifier defaultInternalAccessSpecifier)
        : Container(std::move(parent), std::move(name), accessSpecifier)
          , _constructors()
          , _destructors()
          , _methods()
          , _baseTypes()
          , _currentAccessSpecifier(defaultInternalAccessSpecifier)
    {
    }

    const PtrList<Constructor> & Constructors() const { return _constructors; }
    const PtrList<Destructor> & Destructors() const { return _destructors; }
    const PtrList<Method> & Methods() const { return _methods; }
    const std::vector<Inheritance::Ptr> & BaseTypes() const { return _baseTypes; }

    bool VisitChildren(IASTVisitor & visitor) const
    {
        bool ok = true;
        if (!Container::VisitChildren(visitor))
            ok = false;

        return ok;
    }

    virtual void Add(const Declaration::Ptr & value) override;
    void AddBase(const Inheritance::Ptr & value);
    void SetAccessSpecifier(AccessSpecifier accessSpecifier);

private:
    std::vector<Constructor::Ptr> _constructors;
    std::vector<Destructor::Ptr> _destructors;
    std::vector<Method::Ptr> _methods;
    std::vector<Inheritance::Ptr> _baseTypes;
    AccessSpecifier _currentAccessSpecifier;

    void AddConstructor(const Constructor::Ptr & value);
    void AddDestructor(const Destructor::Ptr & value);
    void AddMethod(const Method::Ptr & value);
};

} // namespace CPPParser
