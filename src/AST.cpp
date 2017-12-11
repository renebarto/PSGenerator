#include "include/Container.h"

#include <fstream>
#include <iomanip>
#include <include/TreeInfo.h>
#include "include/Utility.h"
#include "include/AST.h"
#include "include/Namespace.h"
#include "include/CodeGenerator.h"

using namespace std;
using namespace Utility;

namespace CPPParser
{

template<class T>
bool VisitElement(IASTVisitor & visitor, std::shared_ptr<T> element)
{
    if (element == nullptr)
        return true;
    if (!visitor.Enter(*element))
        return false;
    element->VisitChildren();
    if (!visitor.Leave(*element))
        return false;
    return false;
}

bool AST::Visit(IASTVisitor & visitor) const
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

void AST::Show(std::ostream & stream, int indent) const
{
    TreeInfo treeInfo(stream);
    Visit(treeInfo);
}

void AST::GenerateCode(std::ostream & stream, int indent) const
{
    CodeGenerator codeGenerator(stream);
    Visit(codeGenerator);
}

} // namespace CPPParser
