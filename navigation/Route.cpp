
#include "Route.h"
#include <utility>

Route::Route(Coordinates startPoint, Coordinates endPoint, Meters length, Minutes duration,
             std::vector<std::pair<EntityId, Direction>> ways) : startPoint(startPoint),
                                                                 endPoint(endPoint), length(length),
                                                                 duration(duration),
                                                                 ways(std::move(ways)) {}

Minutes Route::estimatedDuration() const {
    return duration;
}

Meters Route::totalLength() const {
    return length;
}

const Coordinates &Route::getWayStartPoint() const {
    return startPoint;
}

const Coordinates &Route::getWayEndPoint() const {
    return endPoint;
}

const std::vector<std::pair<EntityId, Direction>> &Route::getWays() const {
    return ways;
}

