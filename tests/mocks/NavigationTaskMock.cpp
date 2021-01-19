#include "NavigationTaskMock.h"
#include "NavigationValidatorMock.h"

NavigationTaskMock::NavigationTaskMock(std::unique_ptr<GISContainer> &gisContainer,
                                       std::unique_ptr<NavigationContainer> &navigationContainer,
                                       const NavigationRequest &request, int gisIndex, int navigationIndex,
                                       int requestIndex) : NavigationTask(gisContainer, navigationContainer, request,
                                                                          gisIndex, navigationIndex, requestIndex) {}

const std::unique_ptr<NavigationValidator> &NavigationTaskMock::getValidator() const {
    auto &gis = gisContainer->getGIS();
    validatorMock = std::make_unique<NavigationValidatorMock>(*gis, true);
    return validatorMock;
}
