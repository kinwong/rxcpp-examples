#include "rxcpp/rx.hpp"

namespace rx
{
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::operators;
    using namespace rxcpp::util;
}

namespace sources
{
    void test()
    {
        auto scheduler = rxcpp::observe_on_one_worker(rxcpp::observe_on_new_thread());
        auto start = scheduler.now() + std::chrono::milliseconds(10000);
        auto values = rxcpp::observable<>::timer(start, scheduler);
        values.as_blocking().subscribe(
            [](int v)
            { printf("OnNext: %d\n", v); },
            []()
            { printf("OnCompleted\n"); });
        printf("//! [threaded timepoint timer sample]\n");
    }

    void create_timer()
    {
        auto scheduler = rxcpp::observe_on_new_thread();
        auto start = scheduler.now() + std::chrono::milliseconds(1000);
        auto period = std::chrono::milliseconds(250);
        auto values = rxcpp::observable<>::interval(start, period, scheduler);
        values.take(30).as_blocking().subscribe(
            [](int v)
            { printf("OnNext: %d\n", v); },
            []()
            { printf("OnCompleted\n"); });

        printf("//! [threaded interval sample]\n");
    }

    void test_combine_latest()
    {
        printf("//! [combine_latest sample]\n");
        auto o1 = rxcpp::observable<>::interval(std::chrono::milliseconds(2));
        auto o2 = rxcpp::observable<>::interval(std::chrono::milliseconds(3));
        auto o3 = rxcpp::observable<>::interval(std::chrono::milliseconds(5));
        auto values = o1.combine_latest(o2, o3);
        values.take(50)
            .as_blocking()
            .subscribe(
            [](std::tuple<int, int, int> v)
            { printf("OnNext: %d, %d, %d\n", std::get<0>(v), std::get<1>(v), std::get<2>(v)); },
            []()
            { printf("OnCompleted\n"); });
        printf("//! [combine_latest sample]\n");
    }
}