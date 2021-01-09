#include "NavigationTasksManager.h"

NavigationTasksManager::NavigationTasksManager(int gisAmount, int navigationsAmount, int requestsAmount) : gisAmount(
        gisAmount), navigationsAmount(navigationsAmount), requestsAmount(requestsAmount) {}

NavigationTask NavigationTasksManager::getNextTask() {
    Simulation &sim = Simulation::getInstance();
    auto &gisContainer = sim.getGISContainer(nextGisIndex);
    auto &navigationContainer = sim.getNavigationContainer(nextNavigationIndex);
    auto navigationReq = sim.getNavigationRequest(nextRequestIndex);
    NavigationTask task(gisContainer, navigationContainer, navigationReq);
    if (nextRequestIndex == requestsAmount - 1) {
        nextRequestIndex = 0;
        if (nextNavigationIndex == navigationsAmount - 1) {
            nextNavigationIndex = 0;

            nextGisIndex++;
        } else {
            nextNavigationIndex++;
        }
    } else {
        nextRequestIndex++;
    }
    return task;
}

bool NavigationTasksManager::hasTask() const {
    return nextGisIndex < gisAmount;
}
