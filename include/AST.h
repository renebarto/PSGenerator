#pragma once

#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Container.h"
#include "include/IASTVisitor.h"

using namespace Utility;

namespace CPPParser
{

class AST : public Container
{
public:
    AST()
        : Container(WeakPtr(), "AST", AccessSpecifier::Invalid)
    {}
    virtual bool IsValid() const override { return false; }
    virtual void Show(std::ostream & stream, int indent) const override
    {
        ShowContents(stream, indent);
    }
    virtual void GenerateCode(std::ostream & stream, int indent) const override
    {
        GenerateCodeContents(stream, indent);
    }

    virtual bool Visit(IASTVisitor & visitor) const override;
};

} // namespace CPPParser
