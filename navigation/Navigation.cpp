
#include "Navigation.h"
#include "AStar.h"

Navigation::Navigation(const NavigationGIS &navigationGis) : navigationGIS(navigationGis) {}

Routes Navigation::getRoutes(const Coordinates &start, const Coordinates &end) const {
    return getRoutes(start, end, Restrictions());
}

Routes Navigation::getRoutes(const Coordinates &start, const Coordinates &end, const Restrictions &restrictions) const {
    auto startPair = navigationGIS.getWayClosestPoint(start, restrictions);
    if (startPair.second == EntityId("")) {
        return Routes(Route::invalidRoute(), Route::invalidRoute(), false, "No ways on earth!");
    }
    auto endPair = navigationGIS.getWayClosestPoint(end, restrictions);
    const Way &startWay = navigationGIS.getWay(startPair.second);
    const Coordinates &startPoint = startPair.first;
    const Way &endWay = navigationGIS.getWay(endPair.second);
    const Coordinates &destinationPoint = endPair.first;
    if (startWay.getId() == endWay.getId()) {
        return Routes(Route::invalidRoute(), Route::invalidRoute(), false,
                      "Routes contain only one way!");
    }
    AStar star(navigationGIS, startPoint, destinationPoint, startWay, endWay, restrictions);
    Route shortestByDistance = star.shortestByDistance();
    if (!shortestByDistance.isValid()) {
//        initialize invalid Routes
        return Routes(Route::invalidRoute(startPoint, destinationPoint),
                      Route::invalidRoute(startPoint, destinationPoint), false, "Routes not found!");
    }
    Route shortestByTime = star.shortestByTime();
    return Routes(shortestByDistance, shortestByTime, true);
}

