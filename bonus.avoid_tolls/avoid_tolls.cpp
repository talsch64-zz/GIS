#include "gtest/gtest.h"
#include "../Common/GISNamedTypes.h"
#include "../Navigation/Navigation_315524694.h"
#include "../tests/NavigationValidator.h"
#include "../GIS/entities/Way.h"
#include "../UserCommon/Utils.h"

class IsraelMapTest : public ::testing::Test {
protected:
    GIS_315524694 gis;
    NavigationGIS navGis;
    Navigation_315524694 navigation;
    NavigationValidator validator;

public:
    IsraelMapTest() : gis(GIS_315524694()), navGis(gis), navigation(navGis), validator(gis) {
    }

    void SetUp() override {
        gis.loadMapFile("israel.json");
    }

    void TearDown() override {

    }

    std::vector<EntityId> getWaysIds(const AbstractRoute &route) {
        std::vector<EntityId> ids;
        for (auto pair: route.getWays()) {
            ids.push_back(pair.first);
        }
        return ids;
    }
};



/**
 * origin is J1026 and destination is J1020
 * Four ways exit J1020 but all of them but the slowest one are toll roads.
 * The single route that avoids toll road must go through W2045.
 * The expected output of the test is that the shortestTimeRoute would be different with and without restrictions of "toll"
 */

TEST_F(IsraelMapTest, tollRoadRestrictions) {
    Coordinates origin(Longitude(32.50365),
                       Latitude(35.06183)); // near J1026, closestWayPoint is on a highway (less then 3 meters away)
    Coordinates destination(Longitude(32.10885), Latitude(34.85451)); // J1020

// --- with restrictions ---
    Restrictions restrictions("toll");
    auto restrictedRoutes = navigation.getRoutes(origin, destination, restrictions);

    EXPECT_TRUE(validator.validateRoute(origin, destination, restrictions, restrictedRoutes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(origin, destination, restrictions, restrictedRoutes->shortestTime()));

//    there is only one route that doesn't runs through toll road and it runs through W2045
//    all other routes doesn't go through W2045

    auto shortestTimeWaysRestricted = getWaysIds(restrictedRoutes->shortestTime());
    auto id = std::find(shortestTimeWaysRestricted.begin(), shortestTimeWaysRestricted.end(), EntityId("W2045"));
    EXPECT_NE(id, shortestTimeWaysRestricted.end());

// --- without restrictions ---

    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestTime()));
    auto shortestTimeWays = getWaysIds(routes->shortestTime());
    id = std::find(shortestTimeWays.begin(), shortestTimeWays.end(), EntityId("W2045"));
    EXPECT_EQ(id, shortestTimeWays.end());   // should not include W2045


// without restrictions should run faster!
    EXPECT_NE(routes->shortestTime().estimatedDuration(), restrictedRoutes->shortestTime().estimatedDuration());

}



/**
 * origin is J1026 and destination is J1020
 * Four ways exit J1020 but all of them but the slowest one are highways.
 * The single route that avoids toll road must go through W2045.
 * The expected output of the test is that the shortestTimeRoute would be different with and without restrictions of "highway"
 */

TEST_F(IsraelMapTest, highWayRestrictions) {
    Coordinates origin(Longitude(32.50365),
                       Latitude(35.06183)); // near J1026, closestWayPoint is on a highway (less then 3 meters away)
    Coordinates destination(Longitude(32.10885), Latitude(34.85451)); // J1020

// --- with restrictions ---
    Restrictions restrictions("highway");
    auto restrictedRoutes = navigation.getRoutes(origin, destination, restrictions);

    EXPECT_TRUE(validator.validateRoute(origin, destination, restrictions, restrictedRoutes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(origin, destination, restrictions, restrictedRoutes->shortestTime()));

//    there is only one route that doesn't runs through toll road and it runs through W2045
//    all other routes doesn't go through W2045

    auto shortestTimeWaysRestricted = getWaysIds(restrictedRoutes->shortestTime());
    auto id = std::find(shortestTimeWaysRestricted.begin(), shortestTimeWaysRestricted.end(), EntityId("W2045"));
    EXPECT_NE(id, shortestTimeWaysRestricted.end());

// --- without restrictions ---

    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestTime()));
    auto shortestTimeWays = getWaysIds(routes->shortestTime());
    id = std::find(shortestTimeWays.begin(), shortestTimeWays.end(), EntityId("W2045"));
    EXPECT_EQ(id, shortestTimeWays.end());   // should not include W2045

    EXPECT_NE(routes->shortestTime().estimatedDuration(), restrictedRoutes->shortestTime().estimatedDuration());

}

