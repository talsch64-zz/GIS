
#include "Navigation_315524694.h"
#include "AStar.h"
#include "../Common/NavigationRegistration.h"

REGISTER_NAVIGATION(Navigation_315524694) // Registering the Navigation

Navigation_315524694::Navigation_315524694(const NavigationGIS &navigationGis) : navigationGIS(navigationGis) {}

std::unique_ptr<AbstractRoutes> Navigation_315524694::getRoutes(const Coordinates &start, const Coordinates &end) const {
    return getRoutes(start, end, Restrictions(""));
}

std::unique_ptr<AbstractRoutes>
Navigation_315524694::getRoutes(const Coordinates &start, const Coordinates &end, const Restrictions &restrictions) const {
    auto startPair = navigationGIS.getWayClosestPoint(start, restrictions);
    if (std::get<1>(startPair) == EntityId("")) {
        return std::make_unique<Routes>(nullptr, nullptr, false, "No ways on earth!");
    }
    auto endPair = navigationGIS.getWayClosestPoint(end, restrictions);
    const AbstractWay &startWay = navigationGIS.getWay(std::get<1>(startPair));
    const Coordinates &startPoint = std::get<0>(startPair);
    const AbstractWay &endWay = navigationGIS.getWay(std::get<1>(endPair));
    const Coordinates &destinationPoint = std::get<0>(endPair);
    if (startWay.getId() == endWay.getId()) {
        return std::make_unique<Routes>(nullptr, nullptr, false, "Routes contain only one way!");
    }
    AStar star(navigationGIS, startPoint, destinationPoint, startWay, endWay, restrictions);
    auto shortestByDistance = star.shortestByDistance();
    if (shortestByDistance == nullptr) {
//        initialize invalid Routes
        return std::make_unique<Routes>(nullptr, nullptr, false, "Routes not found!");
    }
    auto shortestByTime = star.shortestByTime();
    return std::make_unique<Routes>(std::move(shortestByDistance), std::move(shortestByTime), true);
}

