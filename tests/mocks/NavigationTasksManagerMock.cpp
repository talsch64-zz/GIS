#include "NavigationTasksManagerMock.h"
#include "NavigationTaskMock.h"

NavigationTasksManagerMock::NavigationTasksManagerMock(int gisAmount, int navigationsAmount, int requestsAmount)
        : NavigationTasksManager(gisAmount, navigationsAmount, requestsAmount),
          fakeResults(
                  std::make_unique<std::unique_ptr<TaskResult>[]>(gisAmount * navigationsAmount * requestsAmount)) {}

std::unique_ptr<NavigationTask> NavigationTasksManagerMock::createTask(std::unique_ptr<GISContainer> &gisContainer,
                                                                       std::unique_ptr<NavigationContainer> &navigationContainer,
                                                                       const NavigationRequest &request, int gisIndex,
                                                                       int navigationIndex, int requestIndex) {
    return std::make_unique<NavigationTaskMock>(gisContainer, navigationContainer,
                                                request, gisIndex,
                                                navigationIndex, requestIndex,
                                                std::move(getResult(gisIndex, navigationIndex, requestIndex)));
}

std::unique_ptr<TaskResult> &
NavigationTasksManagerMock::getResult(int gisIndex, int navigationIndex, int requestIndex) {
    int index = gisIndex * navigationsAmount * requestsAmount +
                navigationIndex * requestsAmount +
                requestIndex;
    return fakeResults[index];
}

void NavigationTasksManagerMock::setResult(int gisIndex, int navigationIndex, int requestIndex,
                                           std::unique_ptr<TaskResult> result) {
    getResult(gisIndex, navigationIndex, requestIndex) = std::move(result);
}
