
#include "Route.h"
#include <utility>

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

bool Route::isValid() const {
    return valid;
}

Route::Route(Coordinates startPoint, Coordinates endPoint, Meters length, Minutes duration,
             std::vector<std::pair<EntityId, Direction>> ways, bool valid) : startPoint(startPoint),
                                                                             endPoint(endPoint), length(length),
                                                                             duration(duration),
                                                                             ways(std::move(ways)), valid(valid) {}

std::unique_ptr<Route> Route::invalidRoute(const Coordinates &start, const Coordinates &end) {
    return  std::make_unique<Route>(start, end, Meters(MAXFLOAT), Minutes(MAXFLOAT), std::vector<std::pair<EntityId, Direction>>(), false);
}

std::unique_ptr<Route> Route::invalidRoute() {
    return std::make_unique<Route>(Coordinates(Longitude(0), Latitude(0)), Coordinates(Longitude(0), Latitude(0)), Meters(MAXFLOAT),
                 Minutes(MAXFLOAT), std::vector<std::pair<EntityId, Direction>>(), false);
}


