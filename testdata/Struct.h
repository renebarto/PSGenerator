#pragma once

namespace NS1 {
namespace NS2 {

struct interface
{
public:
    interface() {}
    virtual ~interface() {}

    virtual void callme() = 0;
};

struct s
{
public:
    s(const interface * i)
        : _interface(i)
    {}
    virtual ~s()
    {}
    const interface * X() const { return _interface; }

private:
    const interface * _interface;
};

} // namespace NS2
} // namespace NS1
