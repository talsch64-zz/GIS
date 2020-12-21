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
RandTestUtils::getBestRoutesDFS(RouteMock *bestTimeRoute, RouteMock *bestDistanceRoute,
                                std::unordered_map<EntityId, Junction *> junctions,
                                std::vector<std::pair<EntityId, Direction>> ways, Junction *start, Junction *end,
                                Junction *current, Meters currentLength, Minutes currentTime) {
    if (current == end) {
        if (!bestTimeRoute->isValid() || currentTime < bestTimeRoute->estimatedDuration()) {
            bestTimeRoute->setDuration(currentTime);
            bestTimeRoute->setLength(currentLength);
            bestTimeRoute->setWays(ways);
        }
        if (!bestDistanceRoute->isValid() || currentLength < bestDistanceRoute->totalLength()) {
            bestDistanceRoute->setDuration(currentTime);
            bestDistanceRoute->setLength(currentLength);
            bestDistanceRoute->setWays(ways);
        }
    } else {
        for (EntityId wayId : current->getWays()) {

        }
    }
    ways.pop_back();
}

