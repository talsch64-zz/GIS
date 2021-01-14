#include <iostream>
#include "TaskResult.h"

void TaskResult::setRoutes(std::unique_ptr<AbstractRoutes> routes) {
    TaskResult::routes = std::move(routes);
}

void TaskResult::setGisUsageCount(size_t gisUsage) {
    TaskResult::gisUsageCount = gisUsage;
}

const std::unique_ptr<AbstractRoutes> &TaskResult::getRoutes() const {
    return routes;
}


std::size_t TaskResult::getGisUsageCount() const {
    return gisUsageCount;
}

bool TaskResult::isShortestDistanceValid() const {
    return shortestDistanceValid;
}

void TaskResult::setShortestDistanceValid(bool shortestDistanceValid) {
    TaskResult::shortestDistanceValid = shortestDistanceValid;
}

bool TaskResult::isShortestTimeValid() const {
    return shortestTimeValid;
}

void TaskResult::setShortestTimeValid(bool shortestTimeValid) {
    TaskResult::shortestTimeValid = shortestTimeValid;
}

bool TaskResult::isValid() const {
    return routes->isValid() && isShortestDistanceValid() && isShortestTimeValid();
}
