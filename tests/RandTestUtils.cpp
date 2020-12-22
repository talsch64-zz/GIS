#include "RandTestUtils.h"

int RandTestUtils::randInt(int min, int max) {
    double f = (double) rand() / RAND_MAX;
    int r = min + (int) std::round(f * (double) (max - min));
    return r;
}

double RandTestUtils::randDouble(double min, double max) {
    double f = (double) rand() / RAND_MAX;
    return min + f * (max - min);
}

bool RandTestUtils::randBool() {
    return randDouble(0, 100) > 50;
}

Coordinates RandTestUtils::randCoord(Bound bound) {
    Latitude lat = Latitude(randDouble((double) bound.minLat, (double) bound.maxLat));
    Longitude lon = Longitude(randDouble((double) bound.minLon, (double) bound.maxLon));
    return Coordinates(lon, lat);
}

Coordinates RandTestUtils::randCoord() {
    return randCoord(Bound(Latitude(-90), Latitude(90), Longitude(-180), Longitude(180)));
}

Bound RandTestUtils::randBound() {
    Coordinates minCoord = randCoord();
    Coordinates dif = randCoord(Bound(Latitude(-1), Latitude(1), Longitude(-1), Longitude(1)));
    auto bound = Bound(minCoord.latitude(), Latitude(minCoord.latitude() + dif.latitude()),
                       minCoord.longitude(), Longitude(minCoord.longitude() + dif.longitude()));
    return bound;
}

std::vector<Junction *>
RandTestUtils::generateJunctions(GISMock &gis, IdGenerator &idGenerator, int n, Bound bound) {
    std::vector<Junction *> junctions;
    for (int i = 0; i < n; i++) {
        EntityId id = idGenerator.generateId();
        Coordinates coord = randCoord(bound);
        std::unique_ptr<Point> point = std::make_unique<Point>(coord);
        std::unique_ptr<Junction> junction = std::make_unique<Junction>(id, "junction", "junction",
                                                                        std::vector<std::string>(), std::move(point));
        junctions.push_back(junction.get());
        gis.addEntity(std::move(junction));
    }
    return junctions;
}

void
RandTestUtils::generateWay(GISMock &gis, const EntityId &id, const std::vector<Coordinates> &curves,
                           Junction &from,
                           Junction &to, const TrafficDirection &dir, int speedLimit) {
    std::vector<Coordinates> pointsCoord = {((Point *) from.getGeometry().get())->getCoordinates()};
    for (const Coordinates &coord : curves) {
        pointsCoord.push_back(coord);
    }
    pointsCoord.push_back(((Point *) to.getGeometry().get())->getCoordinates());
    std::unique_ptr<PointList> points = std::make_unique<PointList>(pointsCoord);
    std::unique_ptr<Way> way = std::make_unique<Way>(id, "way", "way", std::vector<std::string>(),
                                                     std::move(points), from.getId(), to.getId(),
                                                     dir, speedLimit, false, false,
                                                     std::vector<std::string>());
    from.addWay(id);
    if (dir == TrafficDirection::bidirectional) {
        to.addWay(id);
    }
    gis.addEntity(std::move(way));
}

void
RandTestUtils::generateWays(GISMock &gis, IdGenerator &idGenerator, int n, Bound bound,
                            std::vector<Junction *> junctions) {
    for (int i = 0; i < n; i++) {
        EntityId id = idGenerator.generateId();
        Junction *from = junctions[randInt(0, junctions.size() - 1)];
        Junction *to = junctions[randInt(0, junctions.size() - 1)];
        int curvesAmount = randInt(0, 3);
        std::vector<Coordinates> curves;
        for (int j = 0; j < curvesAmount; j++) {
            curves.push_back(randCoord(bound));
        }
        TrafficDirection dir = randBool() ? TrafficDirection::unidirectional : TrafficDirection::bidirectional;
        int speed = randInt(30, 200);
        generateWay(gis, id, curves, *from, *to, dir, speed);
    }
}

void
RandTestUtils::getBestRoutesDFS(NavigationGIS &navGis, RouteMock &bestTimeRoute, RouteMock &bestDistanceRoute,
                                std::vector<std::pair<EntityId, Direction>> ways, const EntityId &start,
                                const EntityId &end, const EntityId &current, Meters currentLength,
                                Minutes currentTime) {
    if (current == end) {
        if (!bestTimeRoute.isValid() || currentTime < bestTimeRoute.estimatedDuration()) {
            bestTimeRoute.setDuration(currentTime);
            bestTimeRoute.setLength(currentLength);
            bestTimeRoute.setWays(ways);
        }
        if (!bestDistanceRoute.isValid() || currentLength < bestDistanceRoute.totalLength()) {
            bestDistanceRoute.setDuration(currentTime);
            bestDistanceRoute.setLength(currentLength);
            bestDistanceRoute.setWays(ways);
        }
    } else {
        for (const EntityId &wayId : navGis.getWaysByJunction(current)) {
            bool alreadyInPath = false;
            for (int i = 0; i < ways.size() && !alreadyInPath; i++) {
                if (ways[i].first == wayId) {
                    alreadyInPath = true;
                }
            }
            if (!alreadyInPath) {
                auto &way = navGis.getWay(wayId);
                Direction dir = current == way.getFromJunctionId() ? Direction::A_to_B : Direction::B_to_A;
                auto waysCopy = ways;
                waysCopy.emplace_back(std::make_pair(wayId, dir));
                EntityId next = current == way.getFromJunctionId() ? way.getToJunctionId() : way.getFromJunctionId();
                Meters newLength = currentLength + way.getLength();
                Minutes newTime = currentTime + Minutes(way.getLength() / 1000 / way.getSpeedLimit() / 60);
                getBestRoutesDFS(navGis, bestTimeRoute, bestDistanceRoute, waysCopy, start, end, next, newLength,
                                 newTime);
            }
        }
    }
}

Routes RandTestUtils::getBestRoutes(GISMock &gis, IdGenerator &idGenerator, RouteMock &bestTimeRoute,
                                    RouteMock &bestDistanceRoute,
                                    const Coordinates &start, const Coordinates &end) {
    auto startWayPair = gis.getWayClosestPoint(start);
    auto endWayPair = gis.getWayClosestPoint(end);
    std::vector<std::pair<EntityId, Direction>> ways;
    const Way &startWay = gis.getWay(startWayPair.second);
    const Way &endWay = gis.getWay(endWayPair.second);
    EntityId startId = startWay.getFromJunctionId();
    EntityId endId = endWay.getToJunctionId();

    EntityId fakeStartJunctionId = idGenerator.generateId();
    std::unique_ptr<Point> startPoint = std::make_unique<Point>(start);
    std::unique_ptr<Junction> fakeStartJunction = std::make_unique<Junction>(fakeStartJunctionId, "junction",
                                                                             "junction",
                                                                             std::vector<std::string>(),
                                                                             std::move(startPoint));

    EntityId fakeStartWayId = idGenerator.generateId();
    std::vector<Coordinates> curves;
    auto fakeStartTo = (Junction *) gis.getEntityById(startWay.getToJunctionId());
    generateWay(gis, fakeStartWayId, curves, *fakeStartJunction, *fakeStartTo,
                TrafficDirection::unidirectional, startWay.getSpeedLimit());

    EntityId fakeStartWayId2 = idGenerator.generateId();
    auto fakeStartTo2 = (Junction *) gis.getEntityById(startWay.getFromJunctionId());
    if (startWay.isBidirectional()) {
        generateWay(gis, fakeStartWayId2, curves, *fakeStartJunction,
                    *fakeStartTo2, TrafficDirection::unidirectional, startWay.getSpeedLimit());
    }
    gis.addEntity(std::move(fakeStartJunction));

    EntityId fakeEndJunctionId = idGenerator.generateId();
    std::unique_ptr<Point> endPoint = std::make_unique<Point>(end);
    std::unique_ptr<Junction> fakeEndJunction = std::make_unique<Junction>(fakeEndJunctionId, "junction", "junction",
                                                                           std::vector<std::string>(),
                                                                           std::move(endPoint));
    EntityId fakeEndWayId = idGenerator.generateId();
    auto fakeEndFrom = (Junction *) gis.getEntityById(endWay.getFromJunctionId());
    generateWay(gis, fakeEndWayId, curves, *fakeEndFrom,
                *fakeEndJunction, TrafficDirection::unidirectional,
                endWay.getSpeedLimit());

    EntityId fakeEndWayId2 = idGenerator.generateId();
    auto fakeEndFrom2 = (Junction *) gis.getEntityById(endWay.getToJunctionId());
    if (endWay.isBidirectional()) {
        generateWay(gis, fakeEndWayId2, curves, *fakeEndFrom2,
                    *fakeEndJunction, TrafficDirection::unidirectional,
                    endWay.getSpeedLimit());
    }
    gis.addEntity(std::move(fakeEndJunction));

    NavigationGIS navGis(gis);
    getBestRoutesDFS(navGis, bestTimeRoute, bestDistanceRoute, ways, fakeStartJunctionId, fakeEndJunctionId,
                     fakeStartJunctionId, Meters(0), Minutes(0));
    if (bestTimeRoute.isValid()) {
        auto timeWays = bestTimeRoute.getWays();
        Direction timeStartWayDir = timeWays.front().first == fakeStartWayId ? Direction::A_to_B : Direction::B_to_A;
        Direction timeEndWayDir = timeWays.back().first == fakeEndWayId ? Direction::A_to_B : Direction::B_to_A;
        timeWays.front().first = startWay.getId();
        timeWays.front().second = timeStartWayDir;
        timeWays.back().first = endWay.getId();
        timeWays.back().second = timeEndWayDir;
        bestTimeRoute.setWays(timeWays);

        auto distanceWays = bestDistanceRoute.getWays();
        Direction distanceStartWayDir =
                distanceWays.front().first == fakeStartWayId ? Direction::A_to_B : Direction::B_to_A;
        Direction distanceEndWayDir = distanceWays.back().first == fakeEndWayId ? Direction::A_to_B : Direction::B_to_A;
        distanceWays.front().first = startWay.getId();
        distanceWays.front().second = distanceStartWayDir;
        distanceWays.back().first = endWay.getId();
        distanceWays.back().second = distanceEndWayDir;
        bestDistanceRoute.setWays(distanceWays);
    }

    Routes routes(bestDistanceRoute, bestTimeRoute, bestDistanceRoute.isValid(), "");
    return routes;
}
