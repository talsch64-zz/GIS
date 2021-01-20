#include "NavigationTasksManagerMock.h"
#include "NavigationTaskMock.h"

NavigationTasksManagerMock::NavigationTasksManagerMock(int gisAmount, int navigationsAmount, int requestsAmount)
        : NavigationTasksManager(gisAmount, navigationsAmount, requestsAmount) {}

std::unique_ptr<NavigationTask> NavigationTasksManagerMock::createTask(std::unique_ptr<GISContainer> &gisContainer,
                                                                       std::unique_ptr<NavigationContainer> &navigationContainer,
                                                                       const NavigationRequest &request, int gisIndex,
                                                                       int navigationIndex, int requestIndex) {
    return std::make_unique<NavigationTaskMock>(gisContainer, navigationContainer,
                                                request, gisIndex,
                                                navigationIndex, requestIndex);
}
