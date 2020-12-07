    #include "gtest/gtest.h"
#include "../GIS.h"
#include "../CoordinatesMath.h"

#define METERS_PRECISION 1

TEST(Search, SearchCircleTest) {
    GIS gis;
    gis.loadMapFile("tests/gearth.json");

    std::vector<EntityId> foundEntityIds = gis.getEntities("aaa",
                                                           Coordinates(Longitude(34.783868), Latitude(31.930202)),
                                                           Meters(1500));

    std::vector<EntityId> expectedEntityIds = {EntityId("1"), EntityId("3"), EntityId("4"), EntityId("5")};
    ASSERT_EQ(foundEntityIds.size(), expectedEntityIds.size());
    for (EntityId id : expectedEntityIds) {
        ASSERT_NE(std::find(foundEntityIds.begin(), foundEntityIds.end(), id), foundEntityIds.end());
    }
}

TEST(Search, SearchWaysTest) {
    GIS gis;
    auto loadedIds = gis.loadMapFile("tests/ways-search.json");

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

TEST(GISBasic, getWayClosestPointTest) {
    GIS gis;
    gis.loadMapFile("tests/russia.json");
    Coordinates coord(Longitude(90.28674), Latitude(65.77863));
    Coordinates closest = gis.getWayClosestPoint(coord).first;
    Coordinates to(Longitude(90.28177), Latitude(65.89199));
    EXPECT_TRUE(CoordinatesMath::calculateDistance(to, closest) < METERS_PRECISION);
}

TEST(GISBasic, getWayClosestPointTest2) {
    GIS gis;
    gis.loadMapFile("tests/russia.json");
    Coordinates coord(Longitude(91.68265), Latitude(65.92547));
    Coordinates closest = gis.getWayClosestPoint(coord).first;
    Coordinates curve(Longitude(90.69101), Latitude(65.98046));
    EXPECT_TRUE(CoordinatesMath::calculateDistance(curve, closest) < METERS_PRECISION);
}
