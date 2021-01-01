

#include "Routes.h"


Routes::Routes(std::unique_ptr<AbstractRoute> shortestDistanceRoute, std::unique_ptr<AbstractRoute> shortestTimeRoute,
               bool valid) : Routes(std::move(shortestDistanceRoute), std::move(shortestTimeRoute), valid, "") {}

Routes::Routes(std::unique_ptr<AbstractRoute> shortestDistanceRoute, std::unique_ptr<AbstractRoute> shortestTimeRoute,
               bool valid,
               const std::string &errorMessage) : shortestDistanceRoute(std::move(shortestDistanceRoute)),
                                                  shortestTimeRoute(std::move(shortestTimeRoute)), valid(valid),
                                                  errorMessage(errorMessage) {}

const AbstractRoute &Routes::shortestDistance() const {
    return *shortestDistanceRoute;
}

const AbstractRoute &Routes::shortestTime() const {
    return *shortestTimeRoute;
}

bool Routes::isValid() const {
    return valid;
}

const std::string &Routes::getErrorMessage() const {
    return errorMessage;
}


