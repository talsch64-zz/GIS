
#include "Navigation_315524694.h"
#include "AStar.h"

Navigation_315524694::Navigation_315524694(const NavigationGIS &navigationGis) : navigationGIS(navigationGis) {}

std::unique_ptr<AbstractRoutes> Navigation_315524694::getRoutes(const Coordinates &start, const Coordinates &end) const {
    return getRoutes(start, end, Restrictions(""));
}

std::unique_ptr<AbstractRoutes>
Navigation_315524694::getRoutes(const Coordinates &start, const Coordinates &end, const Restrictions &restrictions) const {
    auto startTuple = navigationGIS.getWayClosestPoint(start, restrictions);
    if (std::get<AbstractGIS::ClosestPoint::WayId>(startTuple) == EntityId("")) {
        return std::make_unique<Routes>(nullptr, nullptr, false, "No ways on earth!");
    }
    auto endTuple = navigationGIS.getWayClosestPoint(end, restrictions);
    const AbstractWay &startWay = navigationGIS.getWay(std::get<AbstractGIS::ClosestPoint::WayId>(startTuple));
    const Coordinates &startPoint = std::get<AbstractGIS::ClosestPoint::Coord>(startTuple);
    const AbstractWay &endWay = navigationGIS.getWay(std::get<AbstractGIS::ClosestPoint::WayId>(endTuple));
    const Coordinates &destinationPoint = std::get<AbstractGIS::ClosestPoint::Coord>(endTuple);
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

