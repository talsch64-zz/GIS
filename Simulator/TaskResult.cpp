#include <iostream>
#include "TaskResult.h"

TaskResult::TaskResult(std::unique_ptr<AbstractRoutes> routes, bool shortestDistanceValid,
                       bool shortestTimeValid, size_t gisUsageCount) : routes(std::move(routes)),
                                                                       shortestDistanceValid(shortestDistanceValid),
                                                                       shortestTimeValid(shortestTimeValid),
                                                                       gisUsageCount(gisUsageCount) {}

const std::unique_ptr<AbstractRoutes> &TaskResult::getRoutes() const {
    return routes;
}


std::size_t TaskResult::getGisUsageCount() const {
    return gisUsageCount;
}

bool TaskResult::isShortestDistanceValid() const {
    return shortestDistanceValid;
}

bool TaskResult::isShortestTimeValid() const {
    return shortestTimeValid;
}

bool TaskResult::isValid() const {
    return routes->isValid() && isShortestDistanceValid() && isShortestTimeValid();
}
