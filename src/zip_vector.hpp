#include "rxcpp/rx.hpp"

namespace operators
{
    template <typename T>
    rxcpp::observable<std::shared_ptr<std::vector<T>>> zip_vector(
        const std::vector<rxcpp::observable<T>>& observables)
    {
        // map the first observable to convert values to a single-element vector
        auto it = observables.cbegin();

        rxcpp::observable<std::shared_ptr<std::vector<T>>> acc
            = it->map([](T t)
                {
                    return std::make_shared<std::vector<T>>(
                        std::initializer_list<T>{t});
                });

        // fold each observable into accumulator by zipping, and pushing back value
        while (++it != observables.cend())
        {
            acc = acc.zip([](std::shared_ptr<std::vector<T>> acc, T next)
                {
                    acc->push_back(next);
                    return acc;
                },
                *it);
        }
        return acc;
    }
}
