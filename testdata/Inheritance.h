#pragma once

namespace NS1 {
namespace NS2 {

class A
{
public:
    A()
    {}
    virtual ~A()
    {}
    virtual void DoIt() = 0;
};

class B : virtual public A
{
public:
    B()
        : A()
    {}
    virtual ~B()
    {}
    virtual void DoIt() override {}
};

} // namespace NS2
} // namespace NS1
