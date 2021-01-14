
#include <iostream>
#include "Navigation_315524694.h"
#include "AStar.h"
#include "../Common/NavigationRegistration.h"

REGISTER_NAVIGATION(Navigation_315524694) // Registering the Navigation

Navigation_315524694::Navigation_315524694(const NavigationGIS &navigationGis) : AbstractNavigation(navigationGis) {
}

std::unique_ptr<AbstractRoutes> Navigation_315524694::getRoutes(const Coordinates &start, const Coordinates &end) const {
    return getRoutes(start, end, Restrictions(""));
}

std::unique_ptr<AbstractRoutes>
Navigation_315524694::getRoutes(const Coordinates &start, const Coordinates &end, const Restrictions &restrictions) const {
    auto startTuple = gis.getWayClosestPoint(start, restrictions);
    if (std::get<AbstractGIS::ClosestPoint::WayId>(startTuple) == EntityId("")) {
        return std::make_unique<Routes>(nullptr, nullptr, false, "No ways on earth!");
    }
    auto endTuple = gis.getWayClosestPoint(end, restrictions);
    const AbstractWay &startWay = gis.getWay(std::get<AbstractGIS::ClosestPoint::WayId>(startTuple));
    const Coordinates &startPoint = std::get<AbstractGIS::ClosestPoint::Coord>(startTuple);
    const AbstractWay &endWay = gis.getWay(std::get<AbstractGIS::ClosestPoint::WayId>(endTuple));
    const Coordinates &destinationPoint = std::get<AbstractGIS::ClosestPoint::Coord>(endTuple);
    size_t startWaySegment = std::get<AbstractGIS::ClosestPoint::SegmentIndex>(startTuple);
    size_t endWaySegment = std::get<AbstractGIS::ClosestPoint::SegmentIndex>(endTuple);

    AStar star(gis, startPoint, destinationPoint, startWay, startWaySegment, endWay, endWaySegment, restrictions);

    auto shortestByDistance = star.shortestByDistance();
    if (shortestByDistance == nullptr) {
//        initialize invalid Routes
        return std::make_unique<Routes>(nullptr, nullptr, false, "Routes not found!");
    }
    auto shortestByTime = star.shortestByTime();
    return std::make_unique<Routes>(std::move(shortestByDistance), std::move(shortestByTime), true);
}
