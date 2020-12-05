#pragma once

template<typename T>
class NamedType {
    T val;

public:
    explicit NamedType(const T& t): val(t) {}

    operator const T&() const {
        return val;
    }

auto operator<=>(const NamedType<T>&) const = default;
};
