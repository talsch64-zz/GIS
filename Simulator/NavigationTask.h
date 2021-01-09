

#ifndef GIS_CPP_NAVIGATIONTASK_H
#define GIS_CPP_NAVIGATIONTASK_H


#include <memory>
#include "../Common/AbstractGIS.h"
#include "../Common/AbstractNavigation.h"
#include "Simulation.h"
#include "NavigationRequest.h"

class NavigationTask {
    std::unique_ptr<GISContainer> &gisContainer;
    std::unique_ptr<NavigationContainer> &navigationContainer;
    std::unique_ptr<AbstractNavigation> navigation = nullptr;
    const NavigationRequest &request;
    int gisIndex;
    int navigationIndex;
    int requestIndex;

public:
    NavigationTask(std::unique_ptr<GISContainer> &gisContainer,
                   std::unique_ptr<NavigationContainer> &navigationContainer, const NavigationRequest &request,
                   int gisIndex, int navigationIndex, int requestIndex);

    const std::unique_ptr<AbstractNavigation> &getNavigation() const;

    const NavigationRequest &getRequest() const;

    int getGisIndex() const;

    int getNavigationIndex() const;

    int getRequestIndex() const;
};


#endif //GIS_CPP_NAVIGATIONTASK_H
