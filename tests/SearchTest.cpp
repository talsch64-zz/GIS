#include "gtest/gtest.h"
#include "../GIS.h"
#include "GISMock.h"
#include "../entities/Junction.h"
#include "../entities/POI.h"
#include "../entities/Way.h"
#include "../entities/Entity.h"
#include "../entities/geometry/CoordinatesMath.h"
#include "../entities/geometry/Point.h"
#include "../entities/geometry/Circle.h"
#include "../entities/geometry/PointList.h"
#include "../IdGenerator.h"

TEST(Search, SearchCircleTest)
{
    GIS gis;
    gis.loadMapFile("gearth.json");

    std::vector<EntityId> foundEntityIds = gis.getEntities("aaa",
                                                           Coordinates(Longitude(34.783868), Latitude(31.930202)),
                                                           Meters(1500));

    std::vector<EntityId> expectedEntityIds = {EntityId("1"), EntityId("3"), EntityId("4"), EntityId("5")};
    ASSERT_EQ(foundEntityIds.size(), expectedEntityIds.size());
    for (EntityId id : expectedEntityIds)
    {
        ASSERT_NE(std::find(foundEntityIds.begin(), foundEntityIds.end(), id), foundEntityIds.end());
    }
}

TEST(Search, SearchWaysTest)
{
    GIS gis;
    auto loadedIds = gis.loadMapFile("ways-search.json");

    std::vector<EntityId> foundEntityIds = gis.getEntities("way",
                                                           Coordinates(Longitude(-60.550612),
                                                                       Latitude(-23.263132)),
                                                           Meters(130000));

    std::vector<EntityId> expectedEntityIds = {EntityId("1"), EntityId("2"), EntityId("3"), EntityId("12")};
    ASSERT_EQ((int)loadedIds.size(), 15);
    ASSERT_EQ(foundEntityIds.size(), expectedEntityIds.size());
    for (EntityId id : expectedEntityIds)
    {
        ASSERT_NE(std::find(foundEntityIds.begin(), foundEntityIds.end(), id), foundEntityIds.end());
    }
}

std::string ENTITY_NAME = "aaa";

void generateEntity(GISMock *gis, IdGenerator *idGenerator,
                    Latitude minLat, Latitude maxLat, Longitude minLon, Longitude maxLon)
{
    double num = fRand(0, 100);
    Meters maxDistance = CoordinatesMath::calculateDistance(Coordinates(minLon, minLat), Coordinates(maxLon, maxLat));
    std::unique_ptr<Entity> entity;
    if (num < 33)
    {
        Coordinates center = randCoord(minLat, maxLat, minLon, maxLon);
        Meters radius = Meters(fRand(0, maxDistance));
        entity = generatePOI(idGenerator, center, radius);
    }
    else if (num < 66)
    {
        Coordinates coord = randCoord(minLat, maxLat, minLon, maxLon);
        entity = generateJunction(idGenerator, coord);
    }
    else
    {
        Coordinates fromCoord = randCoord(minLat, maxLat, minLon, maxLon);
        std::unique_ptr<Junction> from = generateJunction(idGenerator, fromCoord);
        Coordinates toCoord = randCoord(minLat, maxLat, minLon, maxLon);
        std::unique_ptr<Junction> to = generateJunction(idGenerator, toCoord);
        int curvesAmount = (int)fRand(0, 10);
        std::vector<Coordinates> curves;
        for (int i = 0; i < curvesAmount; i++)
        {
            curves.push_back(randCoord(minLat, maxLat, minLon, maxLon));
        }
        entity = generateWay(idGenerator, curves, from->getId(), to->getId());
        gis->addEntity(std::move(from));
        gis->addEntity(std::move(to));
    }
    gis->addEntity(std::move(entity));
}

std::unique_ptr<Junction> generateJunction(IdGenerator *idGenerator, Coordinates coord)
{
    EntityId id = idGenerator->generateId();
    Point point(coord);
    std::unique_ptr<Junction> junction = std::make_unique<Junction>(id, ENTITY_NAME, ENTITY_NAME, std::vector<std::string>(), point);
    return junction;
}

std::unique_ptr<Way> generateWay(IdGenerator *idGenerator, std::vector<Coordinates> curves, EntityId from, EntityId to)
{
    EntityId id = idGenerator->generateId();
    PointList points(curves);
    std::unique_ptr<Way> way = std::make_unique<Way>(id, ENTITY_NAME, ENTITY_NAME, std::vector<std::string>(), points, from, to,
                                                     "dir", 70, true, std::vector<std::string>());
    return way;
}

std::unique_ptr<POI> generatePOI(IdGenerator *idGenerator, Coordinates center, Meters radius)
{
    EntityId id = idGenerator->generateId();
    Circle circle(center, radius);
    std::unique_ptr<POI> poi = std::make_unique<POI>(id, ENTITY_NAME, ENTITY_NAME, std::vector<std::string>(), std::vector<std::string>(), circle);
    return poi;
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

Coordinates randCoord(Latitude minLat, Latitude maxLat, Longitude minLon, Longitude maxLon)
{
    Latitude lat = Latitude(fRand(minLat, maxLat));
    Longitude lon = Longitude(fRand(minLon, maxLon));
    return Coordinates(lon, lat);
}

Coordinates randCoord()
{
    return randCoord(Latitude(-90), Latitude(90), Longitude(-180), Longitude(180));
}

std::tuple<Latitude, Latitude, Longitude, Longitude> randBound()
{
    Coordinates minCoord = randCoord();
    Coordinates dif = randCoord(Latitude(-45), Latitude(45), Longitude(-90), Longitude(90));
    auto bound = std::make_tuple(minCoord.latitude(), Latitude(minCoord.latitude() + dif.latitude()),
                                 minCoord.longitude(), Longitude(minCoord.longitude() + dif.longitude()));
    return bound;
}

TEST(Search, RandomSearchTest)
{
    std::unique_ptr<IdGenerator> idGenerator = std::make_unique<IdGenerator>();
    int n = 100;
    std::unique_ptr<GISMock> gis = std::make_unique<GISMock>();
    auto bound = randBound();
    for (int i = 0; i < n; i++)
    {
        generateEntity(gis.get(), idGenerator.get(), std::get<0>(bound), std::get<1>(bound),
                       std::get<2>(bound), std::get<3>(bound));
    }
}
