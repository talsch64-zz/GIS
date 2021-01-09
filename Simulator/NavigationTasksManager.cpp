#include "NavigationTasksManager.h"
#include "Simulation.h"

NavigationTasksManager::NavigationTasksManager(int gisAmount, int navigationsAmount, int requestsAmount) : gisAmount(
        gisAmount), navigationsAmount(navigationsAmount), requestsAmount(requestsAmount) {}

std::unique_ptr<NavigationTask> NavigationTasksManager::getNextTask() {
    Simulation &sim = Simulation::getInstance();
    auto &gisContainer = sim.getGISContainer(nextGisIndex);
    auto &navigationContainer = sim.getNavigationContainer(nextNavigationIndex);
    auto navigationReq = sim.getNavigationRequest(nextRequestIndex);
    std::unique_ptr<NavigationTask> task = std::make_unique<NavigationTask>(gisContainer, navigationContainer,
                                                                            navigationReq, nextGisIndex,
                                                                            nextNavigationIndex, nextRequestIndex);
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

int NavigationTasksManager::getTaskIndex(std::unique_ptr<NavigationTask> &task) {
    return task->getGisIndex() * navigationsAmount * requestsAmount +
    task->getNavigationIndex() * requestsAmount +
    task->getRequestIndex();
}
