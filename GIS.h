
#ifndef EX1_GIS_H
#define EX1_GIS_H

#include <utility>
#include <string>
#include <vector>
#include <optional>

template<typename T>
class NamedType {
    T val;
public:
    explicit NamedType(const T& t): val(t) {}
    operator const T&() const {
        return val;
    }
};

struct Longitude: NamedType<double> { using NamedType<double>::NamedType;  };
struct Latitude: NamedType<double> { using NamedType<double>::NamedType; };
struct Meters: NamedType<long> { using NamedType<long>::NamedType; };
using Coordinates = std::pair<Longitude, Latitude>;
struct EntityId: NamedType<std::string> {
    using NamedType<std::string>::NamedType;
};

class GIS {
public:
    std::size_t clear();
    std::vector<EntityId> loadMapFile(const std::string& filename);
    std::size_t saveMapFile(const std::string& filename);
    std::vector<EntityId> getEntities(const std::string& search_name);
    std::vector<EntityId> getEntities(const std::string& search_name, const Coordinates&, Meters radius);
    std::optional<Coordinates> getEntityClosestPoint(const EntityId&, const Coordinates&);
    std::pair<Coordinates, EntityId> getWayClosestPoint(const Coordinates&);
};

#endif //EX1_GIS_H
