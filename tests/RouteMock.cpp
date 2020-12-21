#include "RouteMock.h"

void RouteMock::setLength(Meters length) {
    this->length = length;
}

void RouteMock::setDuration(Minutes duration) {
    this->duration = duration;
}

void RouteMock::setWays(std::vector<std::pair<EntityId, Direction>> ways) {
    this->ways = ways;
}
