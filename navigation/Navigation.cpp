
#include "Navigation.h"
#include "AStar.h"

Navigation::Navigation(const NavigationGIS &navigationGis) : navigationGIS(navigationGis) {}

std::unique_ptr<AbstractRoutes> Navigation::getRoutes(const Coordinates &start, const Coordinates &end) const {
    return getRoutes(start, end, Restrictions(""));
}

std::unique_ptr<AbstractRoutes> Navigation::getRoutes(const Coordinates &start, const Coordinates &end, const Restrictions &restrictions) const {
    auto startPair = navigationGIS.getWayClosestPoint(start, restrictions);
    if (std::get<1>(startPair) == EntityId("")) {
        return std::make_unique<Routes>(Route::invalidRoute(), Route::invalidRoute(), false, "No ways on earth!");
    }
    auto endPair = navigationGIS.getWayClosestPoint(end, restrictions);
    const AbstractWay &startWay = navigationGIS.getWay(std::get<1>(startPair));
    const Coordinates &startPoint = std::get<0>(startPair);
    const AbstractWay &endWay = navigationGIS.getWay(std::get<1>(endPair));
    const Coordinates &destinationPoint = std::get<0>(endPair);
    if (startWay.getId() == endWay.getId()) {
        return std::make_unique<Routes>(Route::invalidRoute(), Route::invalidRoute(), false,
                      "Routes contain only one way!");
    }
    AStar star(navigationGIS, startPoint, destinationPoint, startWay, endWay, restrictions);
    Route shortestByDistance = star.shortestByDistance();
    if (!shortestByDistance.isValid()) {
//        initialize invalid Routes
        return std::make_unique<Routes>(Route::invalidRoute(startPoint, destinationPoint),
                      Route::invalidRoute(startPoint, destinationPoint), false, "Routes not found!");
    }
    Route shortestByTime = star.shortestByTime();
    return std::make_unique<Routes>(shortestByDistance, shortestByTime, true);
}

