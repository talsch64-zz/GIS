

#ifndef GIS_CPP_TASKRESULT_H
#define GIS_CPP_TASKRESULT_H


#include <memory>
#include "../Common/AbstractRoutes.h"


/**
 * class TaskResult represent the result of a task (Navigation x GIS x request)
 * Stores the output Routes of the task, a flag that indicates whether the routes is valid abd the gis usage count.
 */
class TaskResult {
    std::unique_ptr<AbstractRoutes> routes = nullptr;
    bool shortestDistanceValid;
    bool shortestTimeValid;
    std::size_t gisUsageCount;

public:
    TaskResult(std::unique_ptr<AbstractRoutes> routes, bool shortestDistanceValid, bool shortestTimeValid,
               size_t gisUsageCount);

    const std::unique_ptr<AbstractRoutes> &getRoutes() const;

    std::size_t getGisUsageCount() const;

    bool isValid() const;

    bool isShortestDistanceValid() const;

    bool isShortestTimeValid() const;
};


#endif //GIS_CPP_TASKRESULT_H
