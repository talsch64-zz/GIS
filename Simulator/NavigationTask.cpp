#include "NavigationTask.h"
#include "Simulation.h"

NavigationTask::NavigationTask(std::unique_ptr<GISContainer> &gisContainer,
                               std::unique_ptr<NavigationContainer> &navigationContainer,
                               const NavigationRequest &request,
                               int gisIndex, int navigationIndex, int requestIndex) : gisContainer(gisContainer),
                                                                                      navigationContainer(
                                                                                              navigationContainer),
                                                                                      request(request),
                                                                                      gisIndex(gisIndex),
                                                                                      navigationIndex(navigationIndex),
                                                                                      requestIndex(requestIndex) {
    auto &gis = gisContainer->getGIS();
    NavigationGIS navigationGis(*gis);
    navigation = navigationContainer->getNavigation(navigationGis);
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
