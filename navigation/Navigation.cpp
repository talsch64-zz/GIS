
#include "Navigation.h"
#include "AStar.h"

Navigation::Navigation(const NavigationGIS &navigationGis) : navigationGIS(navigationGis) {}

Routes Navigation::getRoutes(const Coordinates &start, const Coordinates &end) const {

    auto startPair = navigationGIS.getWayClosestPoint(start);
    if (startPair.second == EntityId("")) {
        return Routes(Route::invalidRoute(), Route::invalidRoute(), false, "No ways on earth!");
    }
    auto endPair = navigationGIS.getWayClosestPoint(end);
    const Way &startWay = navigationGIS.getWay(startPair.second);
    const Coordinates &startPoint = startPair.first;
    const Way &endWay = navigationGIS.getWay(endPair.second);
    const Coordinates &destinationPoint = endPair.first;
    if (startWay.getId() == endWay.getId() && !startWay.isBidirectional()) {
        return Routes(Route::invalidRoute(), Route::invalidRoute(), false, "Routes contain only one unidirectional way");

    }
    AStar star(navigationGIS, startPoint, destinationPoint, startWay, endWay);
    Route shortestByDistance = star.shortestByDistance();
    if (!shortestByDistance.isValid()) {
//        initialize invalid Routes
        return Routes(Route::invalidRoute(startPoint, destinationPoint), Route::invalidRoute(startPoint, destinationPoint), false, "Routes not found!");
    }
    Route shortestByTime = star.shortestByTime();
    return Routes(shortestByDistance, shortestByTime, true);
}

