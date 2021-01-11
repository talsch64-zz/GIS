#include "NavigationTask.h"
#include "Simulation.h"

NavigationTask::NavigationTask(std::unique_ptr<GISContainer> &gisContainer,
                               std::unique_ptr<NavigationContainer> &navigationContainer,
                               const NavigationRequest &request,
                               int gisIndex, int navigationIndex, int requestIndex) : gisContainer(gisContainer),
                                                                                      request(request),
                                                                                      gisIndex(gisIndex),
                                                                                      navigationIndex(navigationIndex),
                                                                                      requestIndex(requestIndex) {
    auto &gis = gisContainer->getGIS();
    navigationGIS = std::make_unique<NavigationGIS>(*gis);
    navigation = navigationContainer->getNavigation(*navigationGIS);
    validator = std::make_unique<NavigationValidator>(*gis);
}

const std::unique_ptr<AbstractNavigation> &NavigationTask::getNavigation() const {
    return navigation;
}

const NavigationRequest &NavigationTask::getRequest() const {
    return request;
}

int NavigationTask::getGisIndex() const {
    return gisIndex;
}

int NavigationTask::getNavigationIndex() const {
    return navigationIndex;
}

int NavigationTask::getRequestIndex() const {
    return requestIndex;
}

const std::unique_ptr<NavigationValidator> &NavigationTask::getValidator() const {
    return validator;
}

const std::unique_ptr<NavigationGIS> &NavigationTask::getNavigationGis() const {
    return navigationGIS;
}

GISContainer &NavigationTask::getGisContainer() const {
    return *gisContainer;
}
