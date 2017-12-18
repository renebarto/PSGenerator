#pragma once

#include <deque>

namespace CPPParser
{

template<typename T>
class SymbolStack
{
public:
    SymbolStack()
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
        size_t removeCount = std::min(count, _stack.size());
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
            auto value = _stack[_stack.size() - index - 1];
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

} // namespace CPPParser
