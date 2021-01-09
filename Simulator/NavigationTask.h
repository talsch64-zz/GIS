

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

public:
    NavigationTask(std::unique_ptr<GISContainer> &gisContainer,
                   std::unique_ptr<NavigationContainer> &navigationContainer, const NavigationRequest &request);

};


#endif //GIS_CPP_NAVIGATIONTASK_H
