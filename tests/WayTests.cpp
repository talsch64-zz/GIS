#include "gtest/gtest.h"
#include "../GISNamedTypes.h"
#include "../GIS.h"

TEST(GISWays, getWayByJunctionUnidirectional) {
    GIS gis;
    gis.loadMapFile("russia.json");
    EXPECT_EQ(gis.getWaysByJunction(EntityId("J1001")).size(), 1);
    EXPECT_EQ(gis.getWaysByJunction(EntityId("J1002")).size(), 0);
}

//TODO doesn't recognize file although main does
TEST(GISWays, getWayByJunctionBidirectional) {
    GIS gis;
    gis.loadMapFile("russiaBi.json");
    EXPECT_EQ(gis.getWaysByJunction(EntityId("J1001")).size(), 1);
    EXPECT_EQ(gis.getWaysByJunction(EntityId("J1002")).size(), 1);
}

//TODO doesn't recognize file although main does
TEST(GISWays, getWayByJunctionNotJunction) {
    GIS gis;
    gis.loadMapFile("russiaBi.json");
    EXPECT_EQ(gis.getWaysByJunction(EntityId("W1001")).size(), 0);
    EXPECT_EQ(gis.getWaysByJunction(EntityId("empty")).size(), 0);

}