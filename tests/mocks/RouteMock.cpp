#include "RouteMock.h"

RouteMock::RouteMock(const Coordinates &startPoint, const Coordinates &endPoint, const Meters &length,
                     const Minutes &duration, const std::vector<std::pair<EntityId, Direction>> &ways, bool valid)
        : Route(startPoint, endPoint, length, duration, ways), valid(valid) {}

void RouteMock::setLength(Meters length) {
    this->length = length;
}

void RouteMock::setDuration(Minutes duration) {
    this->duration = duration;
}

void RouteMock::setWays(std::vector<std::pair<EntityId, Direction>> ways) {
    this->ways = ways;
}

void RouteMock::setValid(bool valid) {
    this->valid = valid;
}

bool RouteMock::isValid() const {
    return valid;
}

