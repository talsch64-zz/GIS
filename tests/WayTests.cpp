#include "gtest/gtest.h"
#include "../Common/GISNamedTypes.h"
#include "../GIS/GIS.h"

TEST(GISWays, getWayByJunctionUnidirectional) {
    GIS gis;
    gis.loadMapFile("russia.json");
    EXPECT_EQ(gis.getWaysByJunction(EntityId("J1001")).size(), (size_t)1);
    EXPECT_EQ(gis.getWaysByJunction(EntityId("J1002")).size(), (size_t)0);
}

TEST(GISWays, getWayByJunctionBidirectional) {
    GIS gis;
    gis.loadMapFile("russiaBi.json");
    EXPECT_EQ(gis.getWaysByJunction(EntityId("J1001")).size(), (size_t)1);
    EXPECT_EQ(gis.getWaysByJunction(EntityId("J1002")).size(), (size_t)1);
}

//TEST(GISWays, getWayByJunctionNotJunction) {
//    GIS gis;
//    gis.loadMapFile("russiaBi.json");
//    EXPECT_EQ(gis.getWaysByJunction(EntityId("W1001")).size(), (size_t)0);
//    EXPECT_EQ(gis.getWaysByJunction(EntityId("empty")).size(), (size_t)0);
//}