

#include "Routes.h"



Routes::Routes(const Route &shortestDistanceRoute, const Route &shortestTimeRoute, bool valid) : shortestDistanceRoute(
        shortestDistanceRoute), shortestTimeRoute(shortestTimeRoute), valid(valid) {}

const Route &Routes::shortestDistance() const {
    return shortestDistanceRoute;
}

const Route &Routes::shortestTime() const {
    return shortestTimeRoute;
}

bool Routes::isValid() const {
    return valid;
}

