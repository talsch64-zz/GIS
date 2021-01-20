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
protected:
    //allow mocking this method
    virtual std::unique_ptr<NavigationTask> createTask(std::unique_ptr<GISContainer> &gisContainer,
                                                       std::unique_ptr<NavigationContainer> &navigationContainer,
                                                       const NavigationRequest &request,
                                                       int gisIndex, int navigationIndex, int requestIndex);

public:
    NavigationTasksManager(int gisAmount, int navigationsAmount, int requestsAmount);

    std::unique_ptr<NavigationTask> getNextTask();

    bool hasTask() const;

    /**
     * @brief decrements usageCount of the gisContainer of the given task;
     * @param task
     */
    void discardTask(NavigationTask &task);
};


#endif //GIS_CPP_NAVIGATIONTASKSMANAGER_H
