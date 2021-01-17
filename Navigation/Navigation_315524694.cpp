
#include <iostream>
#include "Navigation_315524694.h"
#include "AStar.h"
#include "../Common/NavigationRegistration.h"
#include "../UserCommon/Utils.h"

REGISTER_NAVIGATION(Navigation_315524694) // Registering the Navigation

Navigation_315524694::Navigation_315524694(const NavigationGIS &navigationGis) : AbstractNavigation(navigationGis) {
}

std::unique_ptr<AbstractRoutes>
Navigation_315524694::getRoutes(const Coordinates &start, const Coordinates &end) const {
    return getRoutes(start, end, Restrictions(""));
}

std::unique_ptr<AbstractRoutes>
Navigation_315524694::getRoutes(const Coordinates &start, const Coordinates &end,
                                const Restrictions &restrictions) const {
    try {

        auto startTuple = getClosestValidWay(start, restrictions);
        const Coordinates &startPoint = std::get<0>(startTuple);
        const AbstractWay &startWay = std::get<1>(startTuple);
        const AbstractWay &alternativeStartWay = std::get<3>(startTuple); // the only reason to do it is to beat Gal!

        size_t startWaySegment = std::get<2>(startTuple);

        auto endTuple = getClosestValidWay(end, restrictions);
        const Coordinates &destinationPoint = std::get<0>(endTuple);
        const AbstractWay &endWay = std::get<1>(endTuple);
        const AbstractWay &alternativeEndWay = std::get<3>(endTuple);
        size_t endWaySegment = std::get<2>(endTuple);

        std::unordered_map<EntityId, const AbstractWay &> waysMap; // for caching purposes
        waysMap.insert(std::pair<EntityId, const AbstractWay &>(startWay.getId(), startWay));
        waysMap.insert(std::pair<EntityId, const AbstractWay &>(alternativeStartWay.getId(), alternativeStartWay));
        waysMap.insert(std::pair<EntityId, const AbstractWay &>(endWay.getId(), endWay));
        waysMap.insert(std::pair<EntityId, const AbstractWay &>(alternativeEndWay.getId(), alternativeEndWay));

        AStar star(gis, startPoint, destinationPoint, startWay, startWaySegment, endWay, endWaySegment, waysMap, restrictions);

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

std::tuple<Coordinates, const AbstractWay &, std::size_t, const AbstractWay &>
Navigation_315524694::getClosestValidWay(const Coordinates &coord, const Restrictions &restrictions) const {
    auto wayClosestPointTuple = gis.getWayClosestPoint(coord, restrictions);
    const AbstractWay &way = gis.getWay(std::get<AbstractGIS::ClosestPoint::WayId>(wayClosestPointTuple));
    Coordinates wayCoord = std::get<AbstractGIS::ClosestPoint::Coord>(wayClosestPointTuple);
    std::size_t segment = std::get<AbstractGIS::ClosestPoint::SegmentIndex>(wayClosestPointTuple);
    if (!way.isHighway() || CoordinatesMath::calculateDistance(coord, wayCoord) <= Utils::max_distance_from_highway) {
        // valid way
        return std::tuple<Coordinates, const AbstractWay &, std::size_t, const AbstractWay &>(wayCoord, way, segment, way);
    }
    //TODO the correct solution would be to add the highway to the restriction but there is no API for it.
    Restrictions newRestrictions = restrictions.contains("toll") ? Restrictions("toll, highway") : Restrictions(
            "highway");
    wayClosestPointTuple = gis.getWayClosestPoint(coord, newRestrictions);
    const AbstractWay &newWay = gis.getWay(std::get<AbstractGIS::ClosestPoint::WayId>(wayClosestPointTuple));
    wayCoord = std::get<AbstractGIS::ClosestPoint::Coord>(wayClosestPointTuple);
    segment = std::get<AbstractGIS::ClosestPoint::SegmentIndex>(wayClosestPointTuple);
    return std::tuple<Coordinates, const AbstractWay &, std::size_t, const AbstractWay &>(wayCoord, newWay, segment, way);
}


