#include <sstream>
#include <thread>
#include <chrono>

#include "rxcpp/rx.hpp"

namespace rx
{
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::operators;
    using namespace rxcpp::util;
}

using namespace std::chrono_literals;

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
        auto o1 = rxcpp::observable<>::interval(std::chrono::milliseconds(2), rxcpp::observe_on_new_thread());
        auto o2 = rxcpp::observable<>::interval(std::chrono::milliseconds(3), rxcpp::observe_on_new_thread());
        auto o3 = rxcpp::observable<>::interval(std::chrono::milliseconds(5), rxcpp::observe_on_new_thread());
        auto values = o1.combine_latest(o2, o3);
        auto subscription = values
            //.take(500)
            .subscribe(
            [](std::tuple<int, int, int> v)
            { 
                auto id = std::this_thread::get_id();

                std::stringstream ss;
                ss << "[" << id << "] OnNext: " << std::get<0>(v) << ", " << std::get<1>(v) << ", " << std::get<2>(v) << std::endl;
                printf("%s", ss.str().c_str());

                },
            []()
            { printf("OnCompleted\n"); });

        printf("//! [combine_latest sample]\n");

        std::this_thread::sleep_for(2000ms);

        subscription.clear();
    }
}