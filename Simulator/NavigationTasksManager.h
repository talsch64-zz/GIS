#ifndef GIS_CPP_NAVIGATIONTASKSMANAGER_H
#define GIS_CPP_NAVIGATIONTASKSMANAGER_H


#include "NavigationTask.h"


/**
 * class NavigationTasksManager
 * Responsible for providing NavigationTasks and emulates an iterator.
 * TaskManager maximizes gis utilization: while there are Navigations and requests remaining, it creates tasks with
 * the same gis. This feature helps reduce memory usage because because it minimizes the number of gis'es loaded
 * concurrently.
 */
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

    /**
     * @return True if there is a combination of GIS x Navigation x request that was not executed yet.
     */
    bool hasTask() const;

    /**
     * @brief decrements usageCount of the gisContainer of the given task;
     * @param task
     */
    void discardTask(NavigationTask &task);
};


#endif //GIS_CPP_NAVIGATIONTASKSMANAGER_H
