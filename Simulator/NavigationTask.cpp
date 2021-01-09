//
// Created by student on 09/01/2021.
//

#include "NavigationTask.h"

NavigationTask::NavigationTask(const AbstractGIS &gis, const AbstractNavigation &navigation,
                               const std::pair<Coordinates, Coordinates> &request) : gis(gis), navigation(navigation),
                                                                                     request(request) {}
