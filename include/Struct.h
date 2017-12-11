#pragma once

#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Object.h"

using namespace Utility;

namespace CPPParser
{

class Struct : public Object
{
public:
    using Ptr = std::shared_ptr<Struct>;
    using List = std::vector<Ptr>;

    Struct() = delete;
    explicit Struct(Declaration::WeakPtr parent, std::string name, AccessSpecifier accessSpecifier)
        : Object(std::move(parent), std::move(name), accessSpecifier)
    {}

    virtual bool Visit(IASTVisitor & visitor) const override
    {
        bool ok = true;
        if (!visitor.Enter(*this))
            ok = false;
        if (!VisitChildren(visitor))
            ok = false;
        if (!visitor.Leave(*this))
            ok = false;
        return ok;
    }
    virtual void Show(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << "Struct " << Name() << " {" << std::endl;
        ShowContents(stream, indent + 1);
        stream << Indent(indent) << "}" << std::endl;
    }
    virtual void GenerateCode(std::ostream & stream, int indent) const override
    {
        stream << Indent(indent) << "struct " << Name();
        auto baseTypes = BaseTypes();
        if (!baseTypes.empty())
        {
            bool firstBase = true;
            for (size_t index = 0; index < baseTypes.size(); ++index)
            {
                if (firstBase)
                    stream << " : ";
                else
                    stream << ", ";

                baseTypes[index]->GenerateCode(stream, indent);
                firstBase = false;
            }
        }
        stream << " {" << std::endl;
        GenerateCodeContents(stream, indent);
        stream << Indent(indent) << "}; // struct " << Name() << std::endl;
    }
};

} // namespace CPPParser
