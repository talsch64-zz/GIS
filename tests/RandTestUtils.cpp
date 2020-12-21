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

std::vector<Way *> RandTestUtils::generateWays(std::unique_ptr<GISMock> gis, IdGenerator *idGenerator, int n,
                                               std::vector<Junction *> junctions) {
    std::vector<Way *> ways;
    for (int i = 0; i < n; i++) {
        EntityId id = idGenerator->generateId();
        std::unique_ptr<Junction> from = generateJunction(idGenerator, fromCoord);
        Coordinates toCoord = randCoord(minLat, maxLat, minLon, maxLon);
        std::unique_ptr<Junction> to = generateJunction(idGenerator, toCoord);
        int curvesAmount = (int) fRand(0, 10);
        std::vector<Coordinates> curves;
        for (int i = 0; i < curvesAmount; i++) {
            curves.push_back(randCoord(minLat, maxLat, minLon, maxLon));
        }
        std::unique_ptr<Way> = generateWay(idGenerator, curves, from->getId(), to->getId());
        std::vector<std::string>(), std::move(point));
        junctions.push_back(junction.get());
        gis->addEntity(std::move(junction));
    }
    return junctions;
}


