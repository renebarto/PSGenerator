#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <clang-c/Index.h>
#include "include/Element.h"

using namespace Utility;

namespace CPPParser
{

class Declaration
    : public Element
{
public:
    using Ptr = std::shared_ptr<Declaration>;

    Declaration() = delete;
    explicit Declaration(WeakPtr parent, SourceLocation sourceLocation, std::string name, AccessSpecifier accessSpecifier)
        : Element(std::move(parent), std::move(sourceLocation), std::move(name), accessSpecifier)
    {
    }
    virtual ~Declaration() = default;
};

} // namespace CPPParser
