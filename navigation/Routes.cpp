

#include "Routes.h"



Routes::Routes(const Route &shortestDistanceRoute, const Route &shortestTimeRoute, bool valid) : shortestDistanceRoute(
        shortestDistanceRoute), shortestTimeRoute(shortestTimeRoute), valid(valid), errorMessage("") {}

const Route &Routes::shortestDistance() const {
    return shortestDistanceRoute;
}

const Route &Routes::shortestTime() const {
    return shortestTimeRoute;
}

bool Routes::isValid() const {
    return valid;
}

Routes::Routes(const Route &shortestDistanceRoute, const Route &shortestTimeRoute, bool valid,
               const std::string &errorMessage) : shortestDistanceRoute(shortestDistanceRoute),
                                                  shortestTimeRoute(shortestTimeRoute), valid(valid),
                                                  errorMessage(errorMessage) {}

const std::string &Routes::getErrorMessage() const {
    return errorMessage;
}


