#pragma once

#include <clang-c/Index.h>
#include "include/Declaration.h"
#include "include/IASTVisitor.h"

namespace CPPParser
{

class Enum;
class Function;
class Typedef;

template <class T>
using PtrList = std::vector<std::shared_ptr<T>>;

class Container : public Declaration
{
public:
    using Ptr = std::shared_ptr<Container>;
    using List = std::vector<Ptr>;

    Container() = delete;
    explicit Container(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier)
        : Declaration(std::move(parent), std::move(name), accessSpecifier)
          , _contents()
          , _namespaces()
          , _classes()
          , _structs()
          , _enums()
          , _functions()
          , _typedefs()
    {}
    explicit Container(Declaration::WeakPtr parent, CXCursor token)
        : Declaration(std::move(parent), token)
        , _contents()
        , _namespaces()
        , _classes()
        , _structs()
        , _enums()
        , _functions()
          , _typedefs()
    {}

    const PtrList<Namespace> & Namespaces() const { return _namespaces; }
    const PtrList<Class> & Classes() const { return _classes; }
    const PtrList<Struct> & Structs() const { return _structs; }
    const PtrList<Enum> & Enums() const { return _enums; }
    const PtrList<Function> & Functions() const { return _functions; }
    const PtrList<Typedef> & Typedefs() const { return _typedefs; }

    virtual void Show(std::ostream & stream, int indent) const override = 0;
    virtual void GenerateCode(std::ostream & stream, int indent) const override = 0;
    virtual void ShowContents(std::ostream & stream, int indent) const;
    virtual void GenerateCodeContents(std::ostream & stream, int indent) const;

    virtual void Add(const std::shared_ptr<Declaration> & value);

    bool FindNamespace(const std::string & name, std::shared_ptr<Namespace> & result);
    bool FindClass(const std::string & name, std::shared_ptr<Class> & result);
    bool FindStruct(const std::string & name, std::shared_ptr<Struct> & result);
    bool FindEnum(const std::string & name, std::shared_ptr<Enum> & result);
    bool FindFunction(const std::string & name, std::shared_ptr<Function> & result);
    bool FindTypedef(const std::string & name, std::shared_ptr<Typedef> & result);

    bool VisitChildren(IASTVisitor & visitor) const
    {
        bool ok = true;
        for (auto const & element : _contents)
        {
            if (!element->Visit(visitor))
                ok = false;
        }
        return ok;
    }

protected:
    PtrList<Declaration> _contents;
    PtrList<Namespace> _namespaces;
    PtrList<Class> _classes;
    PtrList<Struct> _structs;
    PtrList<Enum> _enums;
    PtrList<Function> _functions;
    PtrList<Typedef> _typedefs;

private:
    void AddNamespace(const std::shared_ptr<Namespace> & value);
    void AddClass(const std::shared_ptr<Class> & value);
    void AddStruct(const std::shared_ptr<Struct> & value);
    void AddEnum(const std::shared_ptr<Enum> & value);
    void AddFunction(const std::shared_ptr<Function> & value);
    void AddTypedef(const std::shared_ptr<Typedef> & value);
};

} // namespace CPPParser
