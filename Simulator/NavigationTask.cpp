#include "NavigationTask.h"

NavigationTask::NavigationTask(std::unique_ptr<GISContainer> &gisContainer,
                               std::unique_ptr<NavigationContainer> &navigationContainer,
                               const NavigationRequest &request) : gisContainer(gisContainer),
                                                                   navigationContainer(navigationContainer),
                                                                   request(request) {
    auto &gis = gisContainer->getGIS();
    NavigationGIS navigationGis(*gis);
    navigation = navigationContainer->getNavigation(navigationGis);
}
