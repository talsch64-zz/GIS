
#ifndef EX1_DOUBLE_H
#define EX1_DOUBLE_H
#pragma once

#include <cmath>
#include "NamedType.h"

template<std::size_t I>
consteval double tenToThePowerOf() {
    if constexpr(I == 0) return 1;
    else return 10 * tenToThePowerOf<I - 1>();
}

template<std::size_t Presision>
class Double {
    double _val;

    template<std::size_t Presision2>
    friend auto comparisonArgs(Double<Presision> lhs, Double<Presision2> rhs) {
        static constexpr std::size_t actual_precision_factor = std::max(lhs.precision_factor, rhs.precision_factor);
        double val2 = static_cast<double>(rhs);
        return std::pair{static_cast<long>(lhs._val * actual_precision_factor), static_cast<long>(val2 * actual_precision_factor)};
    }

    double roundToPrecision(double val) {
        long whole = static_cast<long>(val);
        double fraction = val - whole;
        double rounded_fraction = std::round(fraction * precision_factor) / precision_factor;
        return whole + rounded_fraction;
    }

public:
    static constexpr double precision_factor = tenToThePowerOf<Presision>();

    Double(double val) : _val(roundToPrecision(val)) {}

    template<std::size_t Presision2>
    friend auto operator<=>(Double<Presision> lhs, Double<Presision2> rhs) {
        auto [val1, val2] = comparisonArgs(lhs, rhs);
        return val1 <=> val2;
    }

    template<std::size_t Presision2>
    friend bool operator==(Double<Presision> lhs, Double<Presision2> rhs) {
        auto [val1, val2] = comparisonArgs(lhs, rhs);
        return val1 == val2;
    }

    operator double() const {
        return _val;
    }
};

template<std::size_t Presision>
struct NamedTypeDouble : NamedType<Double<Presision>> {
    using NamedType<Double<Presision>>::NamedType;
    operator double() const {
        return static_cast<const Double<Presision>&>(*this);
    };
};
#endif //EX1_DOUBLE_H
