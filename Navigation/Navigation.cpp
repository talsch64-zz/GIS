
#include <iostream>
#include "Navigation.h"
#include "AStar.h"
#include "../Common/NavigationRegistration.h"
#include "../UserCommon/Utils.h"

REGISTER_NAVIGATION(Navigation) // Registering the Navigation

Navigation::Navigation(const NavigationGIS &navigationGis) : AbstractNavigation(navigationGis) {
}

std::unique_ptr<AbstractRoutes>
Navigation::getRoutes(const Coordinates &start, const Coordinates &end) const {
    return getRoutes(start, end, Restrictions(""));
}

std::unique_ptr<AbstractRoutes>
Navigation::getRoutes(const Coordinates &start, const Coordinates &end,
                      const Restrictions &restrictions) const {
    try {
        auto startTuple = getClosestValidWay(start, restrictions);
        const Coordinates &startPoint = std::get<0>(startTuple);
        const AbstractWay &startWay = std::get<1>(startTuple);
        size_t startWaySegment = std::get<2>(startTuple);

        auto endTuple = getClosestValidWay(end, restrictions);
        const Coordinates &destinationPoint = std::get<0>(endTuple);
        const AbstractWay &endWay = std::get<1>(endTuple);
        size_t endWaySegment = std::get<2>(endTuple);

        AStar star(gis, startPoint, destinationPoint, startWay, startWaySegment, endWay, endWaySegment, waysMap,
                   waysByJunctionMap, restrictions);

        auto shortestByDistance = star.shortestByDistance();
        if (shortestByDistance == nullptr) {
//        initialize invalid Routes
            return std::make_unique<Routes>(nullptr, nullptr, false, "Routes not found!");
        }
        auto shortestByTime = star.shortestByTime();
        return std::make_unique<Routes>(std::move(shortestByDistance), std::move(shortestByTime), true);
    }
    catch (std::runtime_error &e) {
        // If there is no ways in the gis then exception is thrown
        return std::make_unique<Routes>(nullptr, nullptr, false, "No ways on earth!");
    }
}

std::tuple<Coordinates, const AbstractWay &, std::size_t>
Navigation::getClosestValidWay(const Coordinates &coord, const Restrictions &restrictions) const {
    auto wayClosestPointTuple = gis.getWayClosestPoint(coord, restrictions);
    const AbstractWay &way = gis.getWay(std::get<AbstractGIS::ClosestPoint::WayId>(wayClosestPointTuple));
    Coordinates wayCoord = std::get<AbstractGIS::ClosestPoint::Coord>(wayClosestPointTuple);
    std::size_t segment = std::get<AbstractGIS::ClosestPoint::SegmentIndex>(wayClosestPointTuple);
    waysMap.insert(std::pair<EntityId, const AbstractWay &>(way.getId(), way));
    if (!way.isHighway() || CoordinatesMath::calculateDistance(coord, wayCoord) <= Utils::max_distance_from_highway) {
        // valid way
        return std::tuple<Coordinates, const AbstractWay &, std::size_t>(wayCoord, way, segment);
    }
    //TODO the correct solution would be to add the highway to the restriction but there is no API for it.
    Restrictions newRestrictions = restrictions.contains("toll") ? Restrictions("toll, highway") : Restrictions(
            "highway");
    wayClosestPointTuple = gis.getWayClosestPoint(coord, newRestrictions);
    const AbstractWay &newWay = gis.getWay(std::get<AbstractGIS::ClosestPoint::WayId>(wayClosestPointTuple));
    wayCoord = std::get<AbstractGIS::ClosestPoint::Coord>(wayClosestPointTuple);
    segment = std::get<AbstractGIS::ClosestPoint::SegmentIndex>(wayClosestPointTuple);
    waysMap.insert(std::pair<EntityId, const AbstractWay &>(newWay.getId(), newWay));
    return std::tuple<Coordinates, const AbstractWay &, std::size_t>(wayCoord, newWay, segment);
}


