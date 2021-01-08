#include "NavigationValidator.h"
#include "../Navigation/Route.h"
#include "../UserCommon/entities/Way.h"
#include "../Common/CoordinatesMath.h"
#include "../UserCommon/Utils.h"

NavigationValidator::NavigationValidator(const AbstractGIS &gis) : gis(gis) {
}

bool NavigationValidator::validateRoute(const Coordinates &start, const Coordinates &end,
                                        const AbstractRoute &routes) const {
    return validateRoute(start, end, Restrictions(""), routes);
}

bool
NavigationValidator::validateRoute(const Coordinates &start, const Coordinates &end, const Restrictions &restrictions,
                                   const AbstractRoute &r) const {
    auto startTuple = gis.getWayClosestPoint(start, restrictions);
    auto endTuple = gis.getWayClosestPoint(end, restrictions);

    Coordinates origin = std::get<AbstractGIS::ClosestPoint::Coord>(startTuple);
    Coordinates destination = std::get<AbstractGIS::ClosestPoint::Coord>(endTuple);

    EntityId startWayId = std::get<AbstractGIS::ClosestPoint::WayId>(startTuple);
    EntityId finalWayId = std::get<AbstractGIS::ClosestPoint::WayId>(endTuple);

    const AbstractWay &startWay = gis.getWay(startWayId);
    const AbstractWay &finalWay = gis.getWay(finalWayId);

    if (origin != r.getWayStartPoint() || destination != r.getWayEndPoint()) {
        return false;
    }

    if (startWay.isHighway() &&
        CoordinatesMath::calculateDistance(start, origin) > GIS_315524694::getMaxDistanceFromHighway()) {
        return false;
    }

    if (finalWay.isHighway() &&
        CoordinatesMath::calculateDistance(end, destination) > GIS_315524694::getMaxDistanceFromHighway()) {
        return false;
    }

    if (Utils::isWayRestricted(startWay, restrictions)) {
        return false;
    }

    std::vector<std::pair<EntityId, Direction>> ways = r.getWays();
    if (ways.size() < 1 || ways.front().first != startWayId || ways.back().first != finalWayId) {
        return false;
    }

    Direction initialDirection = ways.front().second;
    Direction finalDirection = ways.back().second;

    if (!startWay.isBidirectional() && initialDirection == Direction::B_to_A) {
        return false; //startWay is unidirectional but the initial direction is B_to_A
    }

    auto startWayIdPair = startWay.getJunctions();
    EntityId currJunction =
            initialDirection == Direction::A_to_B ? startWayIdPair.second : startWayIdPair.first;

    Meters length = startWay.getLength();
    Minutes time = Utils::getWayDuration(startWay.getLength(), startWay.getSpeedLimit());


    EntityId currJunctionValidator("");

    // -------------- validate all the ways found --------------
    for (size_t i = 0; i < ways.size() - 1; i++) {
        EntityId currWayId = ways[i].first;
        Direction currWayDirection = ways[i].second;
        const AbstractWay &currWay = gis.getWay(ways[i].first);
        EntityId nextWayId = ways[i + 1].first;
        Direction nextWayDirection = ways[i + 1].second;
        const AbstractWay &nextWay = gis.getWay(nextWayId);

        auto currWayIdPair = currWay.getJunctions();
        auto nextWayIdPair = nextWay.getJunctions();
        EntityId currJunction =
                currWayDirection == Direction::A_to_B ? currWayIdPair.second : currWayIdPair.first;
        currJunctionValidator =
                nextWayDirection == Direction::A_to_B ? nextWayIdPair.first : nextWayIdPair.second;
        if (currJunctionValidator != currJunction) {
            return false; // currWay and nextWay suppose to share a junction
        }
        if (!nextWay.isBidirectional() && nextWayDirection == Direction::B_to_A) {
            return false;  // next way is unidirectional but the direction is B_to_A
        }
        if (Utils::isWayRestricted(nextWay, restrictions)) {
            return false; // next way is restricted
        }
        length += nextWay.getLength();
        time += Utils::getWayDuration(nextWay.getLength(), nextWay.getSpeedLimit());
    }
    size_t startWaySegment = std::get<AbstractGIS::ClosestPoint::SegmentIndex>(startTuple);
    auto distanceFromStartWayEdges =  startWay.getSegmentPartsOnWay(startWaySegment, origin);
    Meters redundantLengthFromStart =
            initialDirection == Direction::A_to_B ? distanceFromStartWayEdges.first : distanceFromStartWayEdges.second;
    length -= redundantLengthFromStart;
    time -= Utils::calculateTime(redundantLengthFromStart, startWay.getSpeedLimit());

    size_t finalWaySegment = std::get<AbstractGIS::ClosestPoint::SegmentIndex>(endTuple);
    auto distanceFromFinalWayEdges = finalWay.getSegmentPartsOnWay(finalWaySegment, destination);
    Meters redundantLengthFromEnd =
            finalDirection == Direction::A_to_B ? distanceFromFinalWayEdges.second : distanceFromFinalWayEdges.first;
    length -= redundantLengthFromEnd;
    time -= Utils::calculateTime(redundantLengthFromEnd, finalWay.getSpeedLimit());

    if (r.totalLength() != length || r.estimatedDuration() != time) {
        return false;
    }
    return true; // finally!!!
}


//size_t startWaySegment = std::get<AbstractGIS::ClosestPoint::SegmentIndex>(startTuple);
//auto distanceFromStartWayEdges =  startWay.getSegmentPartsOnWay(startWaySegment, start);
//Meters redundantLengthFromStart =
//        initialDirection == Direction::A_to_B ? distanceFromStartWayEdges.first : distanceFromStartWayEdges.second;
//
//length -= redundantLengthFromStart;
//time -= Utils::calculateTime(redundantLengthFromStart, startWay.getSpeedLimit());
//
//
//size_t finalWaySegment = std::get<AbstractGIS::ClosestPoint::SegmentIndex>(endTuple);
//auto distanceFromFinalWayEdges =  finalWay.getSegmentPartsOnWay(finalWaySegment, end);
//
//Meters redundantLengthFromEnd = finalDirection == Direction::A_to_B ? distanceFromFinalWayEdges.second: distanceFromFinalWayEdges.first;