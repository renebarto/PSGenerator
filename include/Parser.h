#pragma once

#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>
#include <cstring>
#include <map>
#include <clang-c/Index.h>
#include "include/Utility.h"
#include "include/Enum.h"
#include "include/Namespace.h"
#include "include/Class.h"
#include "include/Struct.h"
#include "include/AST.h"

namespace CPPParser
{

template<typename T>
class Stack
{
public:
    Stack()
        : _stack()
    {}

    bool AtTop(T element)
    {
        return (_stack.size() >= 1) && (Top() == element);
    }
    void Push(T element)
    {
        _stack.push_back(element);
    }
    T Top()
    {
        return _stack.at(_stack.size() - 1);
    }
    T Pop()
    {
        auto result = Top();
        _stack.pop_back();
        return result;
    }
    void RemoveTopElements(size_t count)
    {
        size_t removeCount = std::max(count, _stack.size());
        for (size_t index = 0; index < removeCount; ++index)
        {
            _stack.pop_back();
        }
    }
    ssize_t Find(T element) const
    {
        // Returns 1 for last element, 2 for one but last, etc., and 0 for not found.
        for (size_t index = 0; index < _stack.size(); ++index)
        {
            CXCursor value = _stack[_stack.size() - index - 1];
            if (element == value)
            {
                return index;
            }
        }
        return -1;
    }
    T At(size_t index) const
    {
        // Index is 0 for last element, 1 for one but last, etc.
        return _stack[_stack.size() - index - 1];
    }
    size_t Count() const
    {
        return _stack.size();
    }

private:
    std::deque<T> _stack;
};

using TokenLookupMap = std::map<CXCursor, Declaration::Ptr>;
using TypeLookupMap = std::map<CXType, Declaration::Ptr>;

class Parser
{
public:
    Parser() = delete;
    explicit Parser(const std::string & path);

    bool Parse(const std::vector<std::string> options);

    const AST & GetAST() const { return _ast; }

    void Show(std::ostream & stream);

    void HandleToken(CXCursor token, CXCursor parentToken);

private:
    std::string _path;
    std::string _fileName;
    AST _ast;
    CXCursor _token;
    CXCursor _parentToken;
    Stack<CXCursor> _stack;
    TokenLookupMap _tokenLookupMap;
    TypeLookupMap _typeLookupMap;

    bool FindNamespaceByName(Declaration::Ptr parent, const std::string & name, Namespace::Ptr & result);
    bool FindClassByName(Declaration::Ptr parent, const std::string & name, Class::Ptr & result);
    bool FindStructByName(Declaration::Ptr parent, const std::string & name, Struct::Ptr & result);
    bool FindEnumByName(Declaration::Ptr parent, const std::string & name, Enum::Ptr & result);

    void AddToMap(CXCursor token, Declaration::Ptr object);
    void AddNamespace(Declaration::Ptr parent, CXCursor token);
    void AddClass(Declaration::Ptr parent, CXCursor token);
    void AddStruct(Declaration::Ptr parent, CXCursor token);
    void AddEnum(Declaration::Ptr parent, CXCursor token);
    void AddEnumValue(Declaration::Ptr parent, CXCursor token);
    void AddConstructor(Declaration::Ptr parent, CXCursor token);
    void AddDestructor(Declaration::Ptr parent, CXCursor token);
    void AddMethod(Declaration::Ptr parent, CXCursor token);
    void AddBaseClass(Declaration::Ptr parent, CXCursor token);
    void AddTypedef(Declaration::Ptr parent, CXCursor token);

    void UpdateStack(Declaration::Ptr object);
    void ShowStack();
};

} // namespace CPPParser
