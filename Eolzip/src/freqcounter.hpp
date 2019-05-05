#ifndef __FREQ_COUNTER_HPP_EOL
#define __FREQ_COUNTER_HPP_EOL

#include <cstddef>
#include <vector>

namespace ett
{
    template <typename T>
    struct FreqUnit
    {
        T obj;
        size_t cnt;
    };

    template <typename T>
    FreqUnit<T>& FU_init(FreqUnit<T>& fu, T obj)
    {
        fu.obj = obj;
        fu.cnt = 1;
        return fu;
    }

    template <typename T>
    struct FreqCounter
    {
        std::vector<FreqUnit<T>> fus;
    };

    template <typename T>
    const std::vector<FreqUnit<T>>& FC_get_fus(const FreqCounter<T>& fcnter)
    {
        return fcnter.fus;
    }

    template <typename T>
    void operator<<(FreqCounter<T>& fcnter, const T& obj)
    {
        bool exist(false);
        for (auto& fu : fcnter.fus)
        {
            if (fu.obj == obj)
            {
                exist = true;
                ++fu.cnt;
                break;
            }
        }
        if (!exist)
        {
            FreqUnit<T> fu;
            FU_init(fu, obj);
            fcnter.fus.emplace_back(fu);
        }
    }

    #ifndef NDEBUG
    #include <iostream>
    template <typename T>
    void FC_print_result(const FreqCounter<T>& fcnter)
    {
        for (const auto& fu : fcnter.fus)
        {
            std::cout << fu.obj << ": " << fu.cnt << std::endl;
        }
    }
    #endif // NDEBUG
}

#endif // __FREQ_COUNTER_HPP_EOL

