

#ifndef GIS_CPP_NAVIGATIONTASK_H
#define GIS_CPP_NAVIGATIONTASK_H


#include <memory>
#include "../Common/AbstractGIS.h"
#include "../Common/AbstractNavigation.h"
#include "Simulation.h"

class NavigationTask {
    const AbstractGIS &gis;
public:
    NavigationTask(const AbstractGIS &gis, const AbstractNavigation &navigation,
                   const std::pair<Coordinates, Coordinates> &request);

private:
    const AbstractNavigation &navigation;
    std::pair<Coordinates, Coordinates> request;
public:

};


#endif //GIS_CPP_NAVIGATIONTASK_H
