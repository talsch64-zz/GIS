//
// Created by student on 10/01/2021.
//

#include <iostream>
#include "TaskResult.h"

void TaskResult::setRoutes(std::unique_ptr<AbstractRoutes> routes) {
    std::cout<< "inside setRoute" << std::endl;
    TaskResult::routes = std::move(routes);
}

void TaskResult::setGisUsageCount(size_t gisUsage) {
    TaskResult::gisUsageCount = gisUsage;
}

const std::unique_ptr<AbstractRoutes> &TaskResult::getRoutes() const {
    return routes;
}


size_t TaskResult::getGisUsageCount() const {
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
