
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

template<typename T>
bool operator==(const NamedType<T> &lhs, const NamedType<T> &rhs) {
    return static_cast<const T &>(lhs) == static_cast<const T &>(rhs);
}

template<typename T>
bool operator<(const NamedType<T> &lhs, const NamedType<T> &rhs) {
    return static_cast<const T &>(lhs) < static_cast<const T &>(rhs);
}

class Coordinates {
    Longitude _longitude;
    Latitude _latitude;

public:
    Coordinates(Longitude longitude, Latitude latitude) : _longitude(longitude), _latitude(latitude) {}

    Longitude longitude() const { return _longitude; }

    Latitude latitude() const { return _latitude; }

    bool operator==(const Coordinates &rhs) const {
        return _longitude == rhs._longitude && _latitude == rhs._latitude;
    }

    template<std::size_t index>
    friend auto get(const Coordinates &c) {
        if constexpr(index == 0) {
            return c.longitude();
        } else {
            return c.latitude();
        }
    }
};

namespace std {
    template<>
    class tuple_size<Coordinates>
            : public std::integral_constant<std::size_t, 2> {
    };

    template<>
    class tuple_element<0, Coordinates> {
    public:
        using type = Longitude;
    };

    template<>
    class tuple_element<1, Coordinates> {
    public:
        using type = Latitude;
    };

    template<>
    struct hash<Coordinates> {
        std::size_t operator()(const Coordinates &c) const {
            std::size_t h1 = std::hash<double>{}(c.longitude());
            std::size_t h2 = std::hash<double>{}(c.latitude());
            return h1 ^ (h2 << 1);
        }
    };
}

struct EntityId : NamedType<std::string> {
    using NamedType<std::string>::NamedType;
};

namespace std {
    template<>
    struct hash<EntityId> {
        std::size_t operator()(const EntityId &t) const {
            return std::hash<std::string>{}(static_cast<const std::string &>(t));
        }
    };
}

#endif //EX1_GISDEFS_H
