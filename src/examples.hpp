#include <iostream>
#include <vector>
#include <chrono>

#include "rxcpp/rx.hpp"
#include "zip_vector.hpp"

namespace examples
{
    void test_combine_latest()
    {
        std::cout << "test_combine_latest" << std::endl;

        auto o1 = rxcpp::observable<>::interval(20ms, rxcpp::observe_on_new_thread());
        auto o2 = rxcpp::observable<>::interval(30ms, rxcpp::observe_on_new_thread());
        auto o3 = rxcpp::observable<>::interval(50ms, rxcpp::observe_on_new_thread());
        auto values = o1.combine_latest(o2, o3);
        auto subscription = values
            .subscribe(
                [](std::tuple<int, int, int> v)
                {
                    auto id = std::this_thread::get_id();
                    std::stringstream ss;
                    ss << "[" << id << "] OnNext: "
                        << std::get<0>(v) << ", "
                        << std::get<1>(v) << ", "
                        << std::get<2>(v);
                    std::cout << ss.str() << std::endl << std::flush;
                },
                []()
                {
                    std::cout << "OnCompleted" << std::endl;
                });

        std::this_thread::sleep_for(200ms);
        subscription.clear();
    }

    void test_zip_vector()
    {
        std::cout << "test_zip_vector" << std::endl;

        std::vector<rxcpp::observable<int>> sources{
            rxcpp::observable<>::range(1, 5),
            rxcpp::observable<>::range(6, 10),
            rxcpp::observable<>::range(11, 15),
            rxcpp::observable<>::range(16, 30)
        };

        operators::zip_vector(sources).
            take(4).
            subscribe(
                [](auto vs) {
                    for (auto& v : *vs) {
                        printf("%d ", v);
                    }
                    printf("\n");
                },
                []() { printf("complete\n"); });
    }

    void test_interval()
    {
        std::cout << "test_interval" << std::endl;

        auto scheduler = rxcpp::observe_on_new_thread();
        auto start = scheduler.now() + 1s;
        auto period = 250ms;
        auto values = rxcpp::observable<>::interval(start, period, scheduler);
        values.take(10).as_blocking().subscribe(
            [](int v)
            {
                std::cout << "OnNext: " << v << std::endl;
            },
            []()
            {
                std::cout << "OnCompleted" << std::endl;
            });
    }

    void test_timer()
    {
        std::cout << "test_timer" << std::endl;
        auto scheduler = rxcpp::observe_on_one_worker(rxcpp::observe_on_new_thread());
        auto start = scheduler.now() + 3s;
        auto values = rxcpp::observable<>::timer(start, scheduler);
        values.as_blocking().subscribe(
            [](int v)
            {
                std::cout << "OnNext: " << v << std::endl;
            },
            []()
            {
                std::cout << "OnCompleted" << std::endl;
            });
    }
}