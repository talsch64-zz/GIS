
#ifndef EX1_GISDEFS_H
#define EX1_GISDEFS_H

#include <utility>
#include <string>
//#include "Double.h"
#include "NamedType.h"


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
    Coordinates(const Longitude &longitude, const Latitude &latitude) : _longitude(trimLongitude(longitude)), _latitude(latitude) {}

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
private:
    Longitude trimLongitude(const Longitude &longitude) {
        double lon = static_cast<const double &>(longitude);
        long whole = static_cast<long>(lon);
        int sign = lon > 0 ? 1 : -1;
        double fraction = abs(lon - whole);
        whole = whole % 360;
        if (whole + fraction > 180) {
            lon = whole - 360 + fraction;
        }
        else if (whole - fraction <= -180){
            lon = whole + 360 - fraction;
        }
        else {
            lon = whole + sign * fraction;
        }
        return Longitude(lon);
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
