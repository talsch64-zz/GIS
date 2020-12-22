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

std::vector<Junction *>
RandTestUtils::generateJunctions(std::unique_ptr<GISMock> gis, IdGenerator *idGenerator, int n, Bound bound) {
    std::vector<Junction *> junctions;
    for (int i = 0; i < n; i++) {
        EntityId id = idGenerator->generateId();
        Coordinates coord = randCoord(bound);
        std::unique_ptr<Point> point = std::make_unique<Point>(coord);
        std::unique_ptr<Junction> junction = std::make_unique<Junction>(id, "junction", "junction",
                                                                        std::vector<std::string>(), std::move(point));
        junctions.push_back(junction.get());
        gis->addEntity(std::move(junction));
    }
    return junctions;
}

std::vector<Way *>
RandTestUtils::generateWays(std::unique_ptr<GISMock> gis, IdGenerator *idGenerator, int n, Bound bound,
                            std::vector<Junction *> junctions) {
    std::vector<Way *> ways;
    for (int i = 0; i < n; i++) {
        EntityId id = idGenerator->generateId();
        Junction *from = junctions[randInt(0, junctions.size() - 1)];
        Junction *to = junctions[randInt(0, junctions.size() - 1)];
        int curvesAmount = randInt(0, 3);
        std::vector<Coordinates> curves;
        for (int i = 0; i < curvesAmount; i++) {
            curves.push_back(randCoord(bound));
        }
        std::unique_ptr<PointList> points = std::make_unique<PointList>(curves);
        TrafficDirection dir = randBool() ? TrafficDirection::unidirectional : TrafficDirection::bidirectional;
        int speed = randInt(30, 200);
        std::unique_ptr<Way> way = std::make_unique<Way>(id, "way", "way", std::vector<std::string>(),
                                                         std::move(points), from->getId(), to->getId(),
                                                         dir, speed, false, false,
                                                         std::vector<std::string>());
        ways.push_back(way.get());
        gis->addEntity(std::move(way));
        from->addWay(id);
        if (dir == TrafficDirection::bidirectional) {
            to->addWay(id);
        }
    }
    return ways;
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
    std::unique_ptr<PointList> points = std::make_unique<PointList>(curves);
    std::unique_ptr<Way> fakeStartWay = std::make_unique<Way>(fakeStartWayId, "way", "way", std::vector<std::string>(),
                                                              std::move(points), fakeStartJunctionId,
                                                              startWay.getToJunctionId(),
                                                              TrafficDirection::unidirectional,
                                                              startWay.getSpeedLimit(), false, false,
                                                              std::vector<std::string>());
    fakeStartJunction->addWay(fakeStartWayId);
    gis.addEntity(std::move(fakeStartWay));

    EntityId fakeStartWayId2 = idGenerator.generateId();
    if (startWay.isBidirectional()) {
        points = std::make_unique<PointList>(curves);
        std::unique_ptr<Way> fakeStartWay2 = std::make_unique<Way>(fakeStartWayId2, "way", "way",
                                                                   std::vector<std::string>(),
                                                                   std::move(points), fakeStartJunctionId,
                                                                   startWay.getFromJunctionId(),
                                                                   TrafficDirection::unidirectional,
                                                                   startWay.getSpeedLimit(), false, false,
                                                                   std::vector<std::string>());
        fakeStartJunction->addWay(fakeStartWayId2);
        gis.addEntity(std::move(fakeStartWay2));
    }
    gis.addEntity(std::move(fakeStartJunction));

    EntityId fakeEndJunctionId = idGenerator.generateId();
    std::unique_ptr<Point> endPoint = std::make_unique<Point>(end);
    std::unique_ptr<Junction> fakeEndJunction = std::make_unique<Junction>(fakeEndJunctionId, "junction", "junction",
                                                                           std::vector<std::string>(),
                                                                           std::move(endPoint));
    EntityId fakeEndWayId = idGenerator.generateId();
    points = std::make_unique<PointList>(curves);
    std::unique_ptr<Way> fakeEndWay = std::make_unique<Way>(fakeEndWayId, "way", "way", std::vector<std::string>(),
                                                            std::move(points), endWay.getFromJunctionId(),
                                                            fakeEndJunctionId,
                                                            TrafficDirection::unidirectional,
                                                            endWay.getSpeedLimit(), false, false,
                                                            std::vector<std::string>());
    fakeEndJunction->addWay(fakeEndWayId);
    gis.addEntity(std::move(fakeEndWay));

    EntityId fakeEndWayId2 = idGenerator.generateId();
    if (endWay.isBidirectional()) {
        points = std::make_unique<PointList>(curves);
        std::unique_ptr<Way> fakeEndWay2 = std::make_unique<Way>(fakeEndWayId2, "way", "way",
                                                                 std::vector<std::string>(),
                                                                 std::move(points), endWay.getToJunctionId(),
                                                                 fakeEndJunctionId,
                                                                 TrafficDirection::unidirectional,
                                                                 endWay.getSpeedLimit(), false, false,
                                                                 std::vector<std::string>());
        fakeEndJunction->addWay(fakeEndWayId2);
        gis.addEntity(std::move(fakeEndWay2));
    }
    gis.addEntity(std::move(fakeEndJunction));

    NavigationGIS navGis(gis);
    getBestRoutesDFS(navGis, bestTimeRoute, bestDistanceRoute, ways, startId, endId, startId, Meters(0), Minutes(0));
    auto timeWays = bestTimeRoute.getWays();
    Direction timeStartWayDir = timeWays.front().first == fakeStartWayId ? Direction::A_to_B : Direction::B_to_A;
    Direction timeEndWayDir = timeWays.back().first == fakeEndWayId ? Direction::A_to_B : Direction::B_to_A;
    timeWays.front().first = startWay.getId();
    timeWays.front().second = timeStartWayDir;
    timeWays.back().first = endWay.getId();
    timeWays.back().second = timeEndWayDir;
    bestTimeRoute.setWays(timeWays);

    auto distanceWays = bestDistanceRoute.getWays();
    Direction distanceStartWayDir = distanceWays.front().first == fakeStartWayId ? Direction::A_to_B : Direction::B_to_A;
    Direction distanceEndWayDir = distanceWays.back().first == fakeEndWayId ? Direction::A_to_B : Direction::B_to_A;
    distanceWays.front().first = startWay.getId();
    distanceWays.front().second = distanceStartWayDir;
    distanceWays.back().first = endWay.getId();
    distanceWays.back().second = distanceEndWayDir;
    bestDistanceRoute.setWays(distanceWays);
    Routes routes(bestDistanceRoute, bestTimeRoute, bestDistanceRoute.isValid(), "");
    return routes;
}
