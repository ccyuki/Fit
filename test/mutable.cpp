#include <fit/mutable.hpp>
#include <fit/lazy.hpp>
#include <fit/detail/move.hpp>
#include <memory>
#include "test.hpp"

struct mutable_fun
{
    int x;
    mutable_fun() noexcept
    : x(1)
    {}

    int operator()(int i) noexcept
    {
        x+=i;
        return x;
    }
};

#if FIT_HAS_NOEXCEPT_DEDUCTION
FIT_TEST_CASE()
{
    static_assert(noexcept(fit::mutable_(mutable_fun())(3)), "noexcept mutable_");
}
#endif

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::mutable_(mutable_fun())(3) == 4);
}

FIT_TEST_CASE()
{
    auto mut_fun = mutable_fun();
    auto by_5 = fit::lazy(fit::mutable_(std::ref(mut_fun)))(5);
    FIT_TEST_CHECK(by_5() == 6);
    FIT_TEST_CHECK(by_5() == 11);
}

struct mutable_move_fun
{
    std::unique_ptr<int> x;
    mutable_move_fun() : x(new int(1))
    {}

    int operator()(int i)
    {
        *x+=i;
        return *x;
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::mutable_(mutable_move_fun())(3) == 4);
}

FIT_TEST_CASE()
{
    auto mut_fun = mutable_move_fun();
    auto by_5 = fit::lazy(fit::mutable_(fit::move(mut_fun)))(5);
    FIT_TEST_CHECK(by_5() == 6);
    FIT_TEST_CHECK(by_5() == 11);
}

