#include "gtest/gtest.h"
#include "../GIS.h"
#include "TestUtils.h"

TEST(Search, SearchCircleTest) {
    GIS gis;
    gis.loadMapFile(TestUtils::getMapFilePath("gearth.json"));

    std::vector<EntityId> foundEntityIds = gis.getEntities("", Coordinates(Longitude(34.783868), Latitude(31.930202)),
                                                           Meters(1500));

    std::vector<EntityId> expectedEntityIds = {EntityId("1"), EntityId("3"), EntityId("4"), EntityId("5")};
    ASSERT_EQ(foundEntityIds.size(), expectedEntityIds.size());
    for (EntityId id : expectedEntityIds) {
        ASSERT_NE(std::find(foundEntityIds.begin(), foundEntityIds.end(), id), foundEntityIds.end());
    }
}