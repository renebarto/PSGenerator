#pragma once

namespace NS1 {

class interface
{
public:
    interface() {}
    virtual ~interface() {}

    virtual void callme() = 0;
};

namespace NS2 {

class c
{
public:
    c(const interface * i)
        : _interface(i)
    {}
    virtual ~c()
    {}
    const interface * X() const { return _interface; }

private:
    const interface * _interface;
};

} // namespace NS2
} // namespace NS1
