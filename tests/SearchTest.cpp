#include "gtest/gtest.h"
#include "../GIS/GIS.h"
#include "mocks/GISMock.h"
#include "../UserCommon/entities/Junction.h"
#include "../UserCommon/entities/POI.h"
#include "../UserCommon/entities/Way.h"
#include "../UserCommon/entities/Entity.h"
#include "../Common/CoordinatesMath.h"
#include "../UserCommon/entities/geometry/Point.h"
#include "../UserCommon/entities/geometry/Circle.h"
#include "../UserCommon/entities/geometry/PointList.h"
#include "../UserCommon/IdGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

TEST(Search, MySearchCircleTest) {
    GIS gis;
    gis.loadMapFile("gearth.json");

    std::vector<EntityId> foundEntityIds = gis.getEntities("aaa",
                                                           Coordinates(Longitude(34.783868), Latitude(31.930202)),
                                                           Meters(1500));

    std::vector<EntityId> expectedEntityIds = {EntityId("1"), EntityId("3"), EntityId("4"), EntityId("5")};
    ASSERT_EQ(foundEntityIds.size(), expectedEntityIds.size());
    for (EntityId id : expectedEntityIds) {
        ASSERT_NE(std::find(foundEntityIds.begin(), foundEntityIds.end(), id), foundEntityIds.end());
    }
}

TEST(Search, MySearchWaysTest) {
    GIS gis;
    auto loadedIds = gis.loadMapFile("ways-search.json");

    std::vector<EntityId> foundEntityIds = gis.getEntities("way",
                                                           Coordinates(Longitude(-60.550612),
                                                                       Latitude(-23.263132)),
                                                           Meters(130000));

    std::vector<EntityId> expectedEntityIds = {EntityId("1"), EntityId("2"), EntityId("3"), EntityId("12")};
    ASSERT_EQ((int) loadedIds.size(), 15);
    ASSERT_EQ(foundEntityIds.size(), expectedEntityIds.size());
    for (EntityId id : expectedEntityIds) {
        ASSERT_NE(std::find(foundEntityIds.begin(), foundEntityIds.end(), id), foundEntityIds.end());
    }
}

std::string getName() {
    return std::string("aaa");
}

double fRand(double fMin, double fMax) {
    double f = (double) rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

Coordinates randCoord(Latitude minLat, Latitude maxLat, Longitude minLon, Longitude maxLon) {
    Latitude lat = Latitude(fRand((double)minLat, (double)maxLat));
    Longitude lon = Longitude(fRand((double)minLon, (double)maxLon));
    return Coordinates(lon, lat);
}

Coordinates randCoord() {
    return randCoord(Latitude(-90), Latitude(90), Longitude(-180), Longitude(180));
}

std::tuple<Latitude, Latitude, Longitude, Longitude> randBound() {
    Coordinates minCoord = randCoord();
    Coordinates dif = randCoord(Latitude(-1), Latitude(1), Longitude(-1), Longitude(1));
    auto bound = std::make_tuple(minCoord.latitude(), Latitude(minCoord.latitude() + dif.latitude()),
                                 minCoord.longitude(), Longitude(minCoord.longitude() + dif.longitude()));
    return bound;
}

std::unique_ptr<Junction> generateJunction(IdGenerator *idGenerator, Coordinates coord) {
    EntityId id = idGenerator->generateId();
    std::unique_ptr<Point> point = std::make_unique<Point>(coord);
    std::unique_ptr<Junction> junction = std::make_unique<Junction>(id, getName(), getName(),
                                                                    std::vector<std::string>(), std::move(point));
    return junction;
}

std::unique_ptr<Way>
generateWay(IdGenerator *idGenerator, std::vector<Coordinates> curves, EntityId from, EntityId to) {
    EntityId id = idGenerator->generateId();
    std::unique_ptr<PointList> points = std::make_unique<PointList>(curves);
    std::unique_ptr<Way> way = std::make_unique<Way>(id, getName(), getName(), std::vector<std::string>(),
                                                     std::move(points), from, to,
                                                     TrafficDirection::unidirectional, 70, true, false, std::vector<std::string>());
    return way;
}

std::unique_ptr<POI> generatePOI(IdGenerator *idGenerator, Coordinates center, Meters radius) {
    EntityId id = idGenerator->generateId();
    std::unique_ptr<Circle> circle = std::make_unique<Circle>(center, radius);
    std::unique_ptr<POI> poi = std::make_unique<POI>(id, getName(), getName(), std::vector<std::string>(),
                                                     std::vector<std::string>(), std::move(circle));
    return poi;
}

void generateEntity(GISMock *gis, IdGenerator *idGenerator,
                    Latitude minLat, Latitude maxLat, Longitude minLon, Longitude maxLon) {
    double num = fRand(0, 100);
    std::unique_ptr<Entity> entity;
    if (num < 33) {
        Meters maxDistance = Meters(
                CoordinatesMath::calculateDistance(Coordinates(minLon, minLat), Coordinates(maxLon, maxLat)) / 3);
        Coordinates center = randCoord(minLat, maxLat, minLon, maxLon);
        Meters radius = Meters(fRand(0, (double)maxDistance));
        entity = generatePOI(idGenerator, center, radius);
    } else if (num < 66) {
        Coordinates coord = randCoord(minLat, maxLat, minLon, maxLon);
        entity = generateJunction(idGenerator, coord);
    } else {
        Coordinates fromCoord = randCoord(minLat, maxLat, minLon, maxLon);
        std::unique_ptr<Junction> from = generateJunction(idGenerator, fromCoord);
        Coordinates toCoord = randCoord(minLat, maxLat, minLon, maxLon);
        std::unique_ptr<Junction> to = generateJunction(idGenerator, toCoord);
        int curvesAmount = (int) fRand(0, 10);
        std::vector<Coordinates> curves;
        for (int i = 0; i < curvesAmount; i++) {
            curves.push_back(randCoord(minLat, maxLat, minLon, maxLon));
        }
        entity = generateWay(idGenerator, curves, from->getId(), to->getId());
        gis->addEntity(std::move(from));
        gis->addEntity(std::move(to));
    }
    gis->addEntity(std::move(entity));
}

TEST(Search, RandomSearchTest) {
    srand(56);

    std::unique_ptr<IdGenerator> idGenerator = std::make_unique<IdGenerator>();
    int n = 100;
    std::unique_ptr<GISMock> gis = std::make_unique<GISMock>();
    auto bound = randBound();
    for (int i = 0; i < n; i++) {
        generateEntity(gis.get(), idGenerator.get(), std::get<0>(bound), std::get<1>(bound),
                       std::get<2>(bound), std::get<3>(bound));
    }
    std::unordered_set<EntityId> inRange;
    int searches = 10;
    for (int i = 0; i < searches; i++) {
        inRange.clear();
        Meters maxDistance = Meters(
                CoordinatesMath::calculateDistance(Coordinates(std::get<2>(bound), std::get<0>(bound)),
                                                   Coordinates(std::get<3>(bound), std::get<1>(bound))) /
                6);
        Coordinates center = randCoord(std::get<0>(bound), std::get<1>(bound),
                                       std::get<2>(bound), std::get<3>(bound));
        Meters radius = Meters(fRand(0, (double)maxDistance));

        auto &entityMap = gis->getEntityMap();
        for (auto &entityPair : entityMap) {
            Entity *entity = entityPair.second.get();
            if (entity->getGeometry()->isInCircle(gis->getTopologicalSearch(), center, radius)) {
                inRange.insert(entity->getId());
            }
        }
        std::vector<EntityId> foundIds = gis->getEntities(getName(), center, radius);
//        std::cout << i << std::endl;
        ASSERT_EQ(inRange.size(), foundIds.size());
        for (EntityId id : foundIds) {
            ASSERT_NE(inRange.find(id), inRange.end());
        }
//        if (inRange.size() != foundIds.size()) {
//            for (EntityId id : foundIds) {
//                inRange.erase(id);
//            }
//            std::cout << "center: (" << center.latitude() << "," << center.longitude() << ")" << std::endl;
//            std::cout << "radius: " << radius << std::endl;
//            std::unique_ptr<GISMock> gis2 = std::make_unique<GISMock>();
//            for (EntityId id : inRange) {
//                auto entity = gis->getEntityById(id);
//                std::unique_ptr<Entity> entity2(entity);
//                gis2->addEntity(std::move(entity2));
//            }
//            gis2->saveMapFile("failed-entities.json");
//        }
    }
}
