#include "gtest/gtest.h"
#include "../GIS.h"

TEST(Search, SearchCircleTest) {
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

TEST(Search, SearchWaysTest) {
    GIS gis;
    auto loadedIds = gis.loadMapFile("ways-search.json");

    std::vector<EntityId> foundEntityIds = gis.getEntities("way",
                                                           Coordinates(Longitude(-59.5938072863849),
                                                                       Latitude(-23.32009705500784)),
                                                           Meters(130000));

    std::vector<EntityId> expectedEntityIds = {EntityId("1"), EntityId("2"), EntityId("3"),
                                               EntityId("10"), EntityId("12")};
    ASSERT_EQ(loadedIds.size(), 15);
    ASSERT_EQ(foundEntityIds.size(), expectedEntityIds.size());
    for (EntityId id : expectedEntityIds) {
        ASSERT_NE(std::find(foundEntityIds.begin(), foundEntityIds.end(), id), foundEntityIds.end());
    }
}