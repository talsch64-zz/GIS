
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

Route::Route(Coordinates startPoint, Coordinates endPoint, Meters length, Minutes duration,
             std::vector<std::pair<EntityId, Direction>> _ways, bool valid) : startPoint(startPoint),
                                                                              endPoint(endPoint), length(length),
                                                                              duration(duration),
                                                                              ways(std::move(_ways)), valid(valid) {}

Route::Route(Coordinates startPoint, Coordinates endPoint) : startPoint(startPoint), endPoint(endPoint), length(MAXFLOAT),
                 duration(MAXFLOAT), ways(std::vector<std::pair<EntityId, Direction>>()), valid(false) {

}


