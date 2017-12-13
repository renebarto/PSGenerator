#pragma once

#include <memory>
#include <vector>
#include "include/Declaration.h"

namespace CPPParser {
namespace Traversal
{

class Element
{
public:
    using Ptr = std::shared_ptr<Element>;
    using List = std::vector<Ptr>;

    Element() = delete;
    explicit Element(Declaration::Ptr item)
        : _item(item)
    {
    }

    const Declaration::Ptr & Item() const
    {
        return _item;
    }

    virtual bool Visit(IASTVisitor & visitor) const
    {
        bool ok = true;
        if (!_item->TraverseBegin(visitor))
            ok = false;
        if (!_item->TraverseEnd(visitor))
            ok = false;
        return ok;
    }

protected:
    Declaration::Ptr _item;
};

class Node
    : public Element
{
public:
    using Ptr = std::shared_ptr<Node>;
    using List = std::vector<Ptr>;

    Node() = delete;
    explicit Node(Declaration::Ptr item)
        : Element(item)
        , _elements()
    {
    }

    void AddNode(Element::Ptr element)
    {
        _elements.push_back(element);
    }

    const Element::List & Elements() const
    {
        return _elements;
    }

    virtual bool Visit(IASTVisitor & visitor) const override
    {
        bool ok = true;
        if ((_item != nullptr) && (!_item->TraverseBegin(visitor)))
            ok = false;
        for (auto const & element : _elements)
        {
            if (!element->Visit(visitor))
                ok = false;
        }
        if ((_item != nullptr) && (!_item->TraverseEnd(visitor)))
            ok = false;
        return ok;
    }

private:
    Element::List _elements;
};

class Tree
    : public Node
{
public:
    Tree()
        : Node(nullptr)
    {}
};

} // namespace Traversal
} // namespace CPPParser
