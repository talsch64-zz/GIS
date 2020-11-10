
#ifndef EX1_GISDEFS_H
#define EX1_GISDEFS_H

#include <utility>
#include <string>

template<typename T>
class NamedType {
    T val;
public:
    explicit NamedType(const T &t) : val(t) {}

    operator const T &() const {
        return val;
    }
};

struct Longitude : NamedType<double> {
    using NamedType<double>::NamedType;
};
struct Latitude : NamedType<double> {
    using NamedType<double>::NamedType;
};
struct Meters : NamedType<double> {
    using NamedType<double>::NamedType;
};
using Coordinates = std::pair<Longitude, Latitude>;
struct EntityId : NamedType<std::string> {
    using NamedType<std::string>::NamedType;
};

#endif //EX1_GISDEFS_H
