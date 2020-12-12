

#ifndef GIS_CPP_ROUTE_H
#define GIS_CPP_ROUTE_H

#include <vector>
#include "../GISNamedTypes.h"

/// Route class describes a route between two points and the ways in which the route goes through..
/// Information available about the Route:
/// * Distance (in Meters)
/// * Duration (in Minutes)
/// * Start and end points
/// * All the ways in which the route goes through (and the directions).
/// * Is the route valid

class Route {
    Coordinates startPoint;
    Coordinates endPoint;
    Meters length;
    Minutes duration;
    std::vector<std::pair<EntityId, Direction>> ways;
    bool valid;

public:
    Route(Coordinates _startPoint, Coordinates _endPoint, Meters _length, Minutes _duration, std::vector<std::pair<EntityId, Direction>> _ways, bool _valid);
    const std::vector<std::pair<EntityId, Direction>>& getWays() const;
    const Coordinates& getWayStartPoint() const;
    const Coordinates& getWayEndPoint() const;
    Meters totalLength() const;
    Minutes estimatedDuration() const;
    bool isValid();
};


#endif //GIS_CPP_ROUTE_H
