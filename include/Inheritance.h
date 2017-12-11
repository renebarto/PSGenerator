#pragma once

#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Declaration.h"

using namespace Utility;

namespace CPPParser
{

class Inheritance
{
public:
    using Ptr = std::shared_ptr<Inheritance>;

    Inheritance() = delete;
    explicit Inheritance(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier,
                         Declaration::WeakPtr base, bool isVirtual)
        : _name(std::move(name))
        , _parent(std::move(parent))
        , _base(std::move(base))
        , _accessSpecifier(accessSpecifier)
        , _isVirtual(isVirtual)
    {
    }

    const std::string & Name() const { return _name; }
    std::shared_ptr<Declaration> Parent() const { return _parent.lock(); }
    AccessSpecifier Access() const { return _accessSpecifier; }
    std::shared_ptr<Declaration> BaseType() const { return _base.lock(); }
    bool IsVirtual() const { return _isVirtual; }

    void Show(std::ostream & stream, int indent) const
    {
        stream << Indent(indent) << "Inheritance " << Name() << std::endl;;
    }
    void GenerateCode(std::ostream & stream, int indent) const
    {
        if (IsVirtual())
            stream << "virtual ";
        stream << Access() << " " << Name();
    }

private:
    std::string _name;
    std::weak_ptr<Declaration> _parent;
    std::weak_ptr<Declaration> _base;
    AccessSpecifier _accessSpecifier;
    bool _isVirtual;
};

} // namespace CPPParser
