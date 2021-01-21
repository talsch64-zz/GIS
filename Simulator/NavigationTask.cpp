#include "NavigationTask.h"
#include "Simulation.h"

NavigationTask::NavigationTask(std::unique_ptr<GISContainer> &gisContainer,
                               std::unique_ptr<NavigationContainer> &navigationContainer,
                               const NavigationRequest &request,
                               int gisIndex, int navigationIndex, int requestIndex) : request(request),
                                                                                      gisIndex(gisIndex),
                                                                                      navigationIndex(navigationIndex),
                                                                                      requestIndex(requestIndex),
                                                                                      gisContainer(gisContainer) {
    auto &gis = gisContainer->getGIS();
    navigationGIS = std::make_unique<NavigationGIS>(*gis);
    navigation = navigationContainer->getNavigation(*navigationGIS);
    validator = std::make_unique<NavigationValidator>(*gis);
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

std::unique_ptr<TaskResult> NavigationTask::execute() {
    auto routes = navigation->getRoutes(request.getFrom(), request.getTo());
    bool validRoutes = routes->isValid();
    bool distanceValid = false, timeValid = false;
    if (validRoutes) {
        auto &shortestDistanceRoute = routes->shortestDistance();
        auto &shortestTimeRoute = routes->shortestTime();
        distanceValid = validator->validateRoute(request.getFrom(), request.getTo(), shortestDistanceRoute);
        timeValid = validator->validateRoute(request.getFrom(), request.getTo(), shortestTimeRoute);
    }
    int gisUsages = navigationGIS->getUsageCounter();
    auto result = std::make_unique<TaskResult>(std::move(routes), distanceValid, timeValid, gisUsages);
    return result;
}

GISContainer &NavigationTask::getGisContainer() const {
    return *gisContainer;
}
