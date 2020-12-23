#include "gtest/gtest.h"
#include "../GIS.h"

TEST(LoadSaveMap, LoadSaveLoadTest) {
    GIS gis, gis2;
    std::vector<EntityId> expectedEntities(
            {EntityId("P1001"), EntityId("P1002"), EntityId("J1001"), EntityId("J1002"), EntityId("J1010"),
             EntityId("P1010"), EntityId("W2001"), EntityId("W2002")});

    gis.loadMapFile("nyc.json");
    gis.saveMapFile("exported-nyc.json");
    std::vector<EntityId> loadedEntities = gis2.loadMapFile("exported-nyc.json");
    remove("exported-nyc.json");

    EXPECT_EQ(loadedEntities, expectedEntities);
}