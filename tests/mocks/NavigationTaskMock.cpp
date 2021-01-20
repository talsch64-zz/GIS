#include "NavigationTaskMock.h"
#include "NavigationValidatorMock.h"

NavigationTaskMock::NavigationTaskMock(std::unique_ptr<GISContainer> &gisContainer,
                                       std::unique_ptr<NavigationContainer> &navigationContainer,
                                       const NavigationRequest &request, int gisIndex, int navigationIndex,
                                       int requestIndex, std::unique_ptr<TaskResult> fakeResult) : NavigationTask(
        gisContainer, navigationContainer, request,
        gisIndex, navigationIndex, requestIndex), fakeResult(std::move(fakeResult)) {}

std::unique_ptr<TaskResult> NavigationTaskMock::execute() {
    return std::move(fakeResult);
}
