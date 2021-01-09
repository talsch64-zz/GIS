#ifndef GIS_CPP_NAVIGATIONTASKSMANAGER_H
#define GIS_CPP_NAVIGATIONTASKSMANAGER_H


#include "NavigationTask.h"

class NavigationTasksManager {
    int nextGisIndex = 0;
    int nextNavigationIndex = 0;
    int nextRequestIndex = 0;
    int gisAmount;
    int navigationsAmount;
    int requestsAmount;
public:
    NavigationTasksManager(int gisAmount, int navigationsAmount, int requestsAmount);

    std::unique_ptr<NavigationTask> getNextTask();

    bool hasTask() const;

    int getTaskIndex(std::unique_ptr<NavigationTask> &task);
};


#endif //GIS_CPP_NAVIGATIONTASKSMANAGER_H
