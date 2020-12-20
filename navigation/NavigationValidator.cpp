//
// Created by student on 20/12/2020.
//

#include "NavigationValidator.h"
#include "Route.h"
#include "../entities/Way.h"
#include "../CoordinatesMath.h"
#include "../Utils.h"

NavigationValidator::NavigationValidator(const GIS &gis): gis(gis) {
}

bool NavigationValidator::validateRoute(const Coordinates &start, const Coordinates &end, const Route &r) const {
    if (!r.isValid()) {
        return false;
    }
    std::pair<Coordinates, EntityId> startPair = gis.getWayClosestPoint(start);
    std::pair<Coordinates, EntityId> endPair = gis.getWayClosestPoint(end);

    Coordinates origin = startPair.first;
    Coordinates destination = endPair.first;

    EntityId startWayId = startPair.second;
    EntityId finalWayId = endPair.second;

    const Way &startWay = gis.getWay(startWayId);
    const Way &finalWay = gis.getWay(endPair.second);


    if (origin != r.getWayStartPoint() || destination != r.getWayEndPoint()) {
        return false;
    }

    if (startWay.isHighway() &&
        CoordinatesMath::calculateDistance(start, origin) > gis.getMaxDistanceFromHighway()) {
        return false;
    }

    std::vector<std::pair<EntityId, Direction>> ways = r.getWays();
    if (ways.size() == 0 || ways.front().first != startWayId || ways.back().first != finalWayId) {
        return false;
    }

    Direction initialDirection = ways.front().second;
    Direction finalDirection = ways.back().second;

    if (!startWay.isBidirectional() && initialDirection == Direction::B_to_A) {
        return false; //startWay is unidirectional but the initial direction is B_to_A
    }

    EntityId currJunction =
            initialDirection == Direction::A_to_B ? startWay.getToJunctionId() : startWay.getFromJunctionId();

    Meters length = startWay.getLength();
    Minutes time = startWay.getTime();


    EntityId currJunctionValidator("");

    // -------------- validate all the ways found --------------
    for (int i = 0; i < ways.size() - 1; i++) {
        EntityId currWayId = ways[i].first;
        Direction currWayDirection = ways[i].second;
        const Way &currWay = gis.getWay(ways[i].first);
        EntityId nextWayId = ways[i + 1].first;
        Direction nextWayDirection = ways[i + 1].second;
        const Way &nextWay = gis.getWay(nextWayId);


        EntityId currJunction =
                currWayDirection == Direction::A_to_B ? currWay.getToJunctionId() : currWay.getFromJunctionId();
        currJunctionValidator =
                nextWayDirection == Direction::A_to_B ? nextWay.getFromJunctionId() : nextWay.getToJunctionId();
        if (currJunctionValidator != currJunction) {
            return false; // currWay and nextWay suppose to share a junction
        }
        if (!nextWay.isBidirectional() && nextWayDirection == Direction::B_to_A) {
            return false;  // next way is unidirectional but the direction is B_to_A
        }
        length += nextWay.getLength();
        time += nextWay.getTime();
    }

    Meters redundantLengthFromStart =
            initialDirection == Direction::A_to_B ? CoordinatesMath::calculateDistance(origin,
                                                                                       startWay.getFromJunctionCoordinates())
                                                  : CoordinatesMath::calculateDistance(origin,
                                                                                       startWay.getToJunctionCoordinates());
    length -= redundantLengthFromStart;
    time -= calculateTime(redundantLengthFromStart, startWay.getSpeedLimit());


    Meters redundantLengthFromEnd =
            finalDirection == Direction::A_to_B ? CoordinatesMath::calculateDistance(destination,
                                                                                     finalWay.getToJunctionCoordinates())
                                                : CoordinatesMath::calculateDistance(destination,
                                                                                     finalWay.getFromJunctionCoordinates());
    length -= redundantLengthFromEnd;
    time -= calculateTime(redundantLengthFromEnd, finalWay.getSpeedLimit());


    if (r.totalLength() != length || r.estimatedDuration() != time) {
        return false;
    }


    return true; // finally!!!
}

bool NavigationValidator::validateRoute(const Coordinates &start, const Coordinates &end, const Restrictions &res,
                                        const Route &r) const {
    return false;
}
