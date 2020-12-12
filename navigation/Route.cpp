
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

bool Route::isValid() {
    return valid;
}

Route::Route(Coordinates _startPoint, Coordinates _endPoint, Meters _length, Minutes _duration,
             std::vector<std::pair<EntityId, Direction>> _ways, bool _valid): startPoint(_startPoint), endPoint(_endPoint), length(_length),
    duration(_duration), ways(std::move(_ways)), valid(_valid) {}


