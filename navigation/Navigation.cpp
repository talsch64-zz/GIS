
#include "Navigation.h"
#include "AStar.h"

Navigation::Navigation(const NavigationGIS &navigationGis) : navigationGIS(navigationGis) {}

Routes Navigation::getRoutes(const Coordinates &start, const Coordinates &end) const {

    auto startPair = navigationGIS.getWayClosestPoint(start);
    if (startPair.second == EntityId("")) {
//        TODO return invalid Routes because there is no ways on earth
    }

    auto endPair = navigationGIS.getWayClosestPoint(end);
    if (endPair.second == EntityId("")) {
//        TODO return invalid Routes because there is no ways on earth
    }
    const Way &startWay = navigationGIS.getWay(startPair.second);
    const Coordinates &startPoint = startPair.first;
    const Way &endWay = navigationGIS.getWay(endPair.second);
    const Coordinates &destinationPoint = endPair.first;
    AStar star(navigationGIS, startPoint, destinationPoint, startWay, endWay);

    Route shortestByDistance = star.shortestByDistance();
    if (!shortestByDistance.isValid()) {
//        initialize invalid Routes
        return Routes(Route(startPoint, destinationPoint), Route(startPoint, destinationPoint), false);
    }
    Route shortestByTime = star.shortestByTime();
    return Routes(shortestByDistance, shortestByTime, true);
}

