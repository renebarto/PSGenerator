#pragma once

#include <cstddef>

namespace NS1 {

template<class T>
class A {
public:
    A(std::size_t n);

    const T * Data() const;

private:
    T * _data;
};

} // namespace NS1
