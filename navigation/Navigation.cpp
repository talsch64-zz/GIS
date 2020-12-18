
#include "Navigation.h"
#include "AStar.h"

Navigation::Navigation(const NavigationGIS &gis) : gis(gis) {}

Routes Navigation::getRoutes(const Coordinates &start, const Coordinates &end) const {
    AStar star;
    Route shortestByDistance = star.
    return Routes(Route(), Route(), false);
}

