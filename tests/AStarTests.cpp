
#include "gtest/gtest.h"
#include "../Common/GISNamedTypes.h"
#include "../GISProvider.h"
#include "../Common/CoordinatesMath.h"
#include "../Navigation/Navigation_315524694.h"
#include "NavigationValidator.h"
#include "../GIS/entities/Way.h"
#include "../UserCommon/Utils.h"

#define DISTANCE_PRECISION 5

class IsraelMapTest : public ::testing::Test {
protected:
    GIS_315524694 gis;
    NavigationGIS navGis;
    Navigation_315524694 navigation;
    NavigationValidator validator;

public:
    IsraelMapTest() : gis(GISProvider::getGIS()), navGis(gis), navigation(navGis), validator(gis) {
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

    void printRoutes(const AbstractRoutes &routes) {
        if (!routes.isValid()) {
            std::cout << "invalid routes!!" << std::endl;
            return;
        }
        std::cout << std::endl << "---ShortestDistance Route--- " << std::endl;

        for (const auto &way : routes.shortestDistance().getWays()) {
            std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
                      << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
            std::cout << "length: " << (double) gis.getWay(way.first).getLength() << ", time: "
                      << (double) Utils::getWayDuration(gis.getWay(way.first).getLength(),
                                                        gis.getWay(way.first).getSpeedLimit()) << std::endl;
            std::cout << "==================================" << std::endl;
        }

        std::cout << std::endl << "---ShortestTime Route--- " << std::endl;

        for (const auto &way : routes.shortestTime().getWays()) {
            std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
                      << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
            std::cout << "length: " << (double) gis.getWay(way.first).getLength() << ", time: "
                      << (double) Utils::getWayDuration(gis.getWay(way.first).getLength(),
                                                        gis.getWay(way.first).getSpeedLimit()) << std::endl;
            std::cout << "==================================" << std::endl;
        }
        std::cout << "ShortestDistanceRoute - distance: " << (double) routes.shortestDistance().totalLength()
                  << ", time: "
                  << (double) routes.shortestDistance().estimatedDuration() << std::endl;
        std::cout << "ShortestTimeRoute - distance: " << (double) routes.shortestTime().totalLength() << ", time: "
                  << (double) routes.shortestTime().estimatedDuration() << std::endl;
    }
};


TEST_F(IsraelMapTest, getSegmentPartsOnWayTest1) {
    Coordinates coordinates(Longitude(31.99435), Latitude(35.49579));
    auto tuple = gis.getWayClosestPoint(coordinates);
    EXPECT_EQ(std::get<AbstractGIS::ClosestPoint::WayId>(tuple), EntityId("W2060"));
    EXPECT_EQ(std::get<AbstractGIS::ClosestPoint::SegmentIndex>(tuple), 0);
    auto &way = gis.getWay(EntityId("W2060"));
    auto distancePair = way.getSegmentPartsOnWay(std::get<AbstractGIS::ClosestPoint::SegmentIndex>(tuple), coordinates);
    Meters distance = distancePair.first + distancePair.second;
    Meters length = way.getLength();
    EXPECT_LE(std::fabs(static_cast<double>(distance - length)), DISTANCE_PRECISION);

    // check segment equals to -1 if the coordinates are not located on the way
    auto way2 = dynamic_cast<Way *>(gis.getEntityById(EntityId("W2060")));
    EXPECT_EQ(way2->getContainingSegment(Coordinates(Longitude(0), Latitude(0))), -1);
}


TEST_F(IsraelMapTest, getSegmentPartsOnWayTest2) {
    Coordinates c0(Longitude(32.14238), Latitude(34.9836));
    Coordinates c1(Longitude(32.11719), Latitude(34.89932));
    Coordinates c2(Longitude(32.10992), Latitude(34.8734));
    auto tuple0 = gis.getWayClosestPoint(c0);
    auto tuple1 = gis.getWayClosestPoint(c1);
    auto tuple2 = gis.getWayClosestPoint(c2);


    EXPECT_EQ(std::get<AbstractGIS::ClosestPoint::WayId>(tuple0), EntityId("W2035"));
    EXPECT_EQ(std::get<AbstractGIS::ClosestPoint::SegmentIndex>(tuple0), 0);
    EXPECT_EQ(std::get<AbstractGIS::ClosestPoint::WayId>(tuple1), EntityId("W2035"));
    EXPECT_EQ(std::get<AbstractGIS::ClosestPoint::SegmentIndex>(tuple1), 1);
    EXPECT_EQ(std::get<AbstractGIS::ClosestPoint::WayId>(tuple2), EntityId("W2035"));
    EXPECT_EQ(std::get<AbstractGIS::ClosestPoint::SegmentIndex>(tuple2), 2);


    auto &way = gis.getWay(EntityId("W2035"));
    auto distancePair0 = way.getSegmentPartsOnWay(std::get<AbstractGIS::ClosestPoint::SegmentIndex>(tuple0), c0);
    auto distancePair1 = way.getSegmentPartsOnWay(std::get<AbstractGIS::ClosestPoint::SegmentIndex>(tuple1), c1);
    auto distancePair2 = way.getSegmentPartsOnWay(std::get<AbstractGIS::ClosestPoint::SegmentIndex>(tuple2), c2);


    Meters distance0 = distancePair0.first + distancePair0.second;
    Meters distance1 = distancePair1.first + distancePair1.second;
    Meters distance2 = distancePair2.first + distancePair2.second;


    Meters length = way.getLength();
    EXPECT_LE(std::fabs(static_cast<double>(distance0 - length)), DISTANCE_PRECISION);
    EXPECT_LE(std::fabs(static_cast<double>(distance1 - length)), DISTANCE_PRECISION);
    EXPECT_LE(std::fabs(static_cast<double>(distance2 - length)), DISTANCE_PRECISION);
}


/**
 * Routes should be the same but reveres
 */
TEST_F(IsraelMapTest, oppositeRoutesByDistance) {
    Coordinates from(Longitude(32.113357), Latitude(34.801290)); // J1001
    Coordinates to(Longitude(32.11265), Latitude(34.79254)); // J1014
    auto routes = navigation.getRoutes(from, to);
    EXPECT_TRUE(routes->isValid());

    EXPECT_TRUE(validator.validateRoute(from, to, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(from, to, routes->shortestTime()));


    auto size1 = routes->shortestDistance().getWays().size();
    auto reverseRoutes = navigation.getRoutes(to, from);
    EXPECT_TRUE(routes->isValid());
    auto size2 = routes->shortestDistance().getWays().size();
    EXPECT_EQ(size1, size2);
    auto from_to_ways = routes->shortestDistance().getWays();
    auto to_from_ways = reverseRoutes->shortestDistance().getWays();
    for (int i = 0; i < (int) size1; i++) {
        EXPECT_EQ(from_to_ways[i].first, to_from_ways[(int) size1 - 1 - i].first);
        EXPECT_NE(from_to_ways[i].second, to_from_ways[(int) size1 - 1 - i].second);
    }
}


/**
 * Routes should be the same but reveres
 */
TEST_F(IsraelMapTest, oppositeRoutesByTime) {
    Coordinates from(Longitude(32.113357), Latitude(34.801290)); // J1001
    Coordinates to(Longitude(32.11265), Latitude(34.79254)); // J1014
    auto routes = navigation.getRoutes(from, to);

    EXPECT_TRUE(validator.validateRoute(from, to, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(from, to, routes->shortestTime()));

    auto size1 = routes->shortestTime().getWays().size();

    auto reverseRoutes = navigation.getRoutes(to, from);
    EXPECT_TRUE(validator.validateRoute(to, from, reverseRoutes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(to, from, reverseRoutes->shortestTime()));

    auto size2 = routes->shortestTime().getWays().size();
    EXPECT_EQ(size1, size2);

    auto from_to_ways = routes->shortestTime().getWays();
    auto to_from_ways = reverseRoutes->shortestTime().getWays();
    for (int i = 0; i < (int) size1; i++) {
        EXPECT_EQ(from_to_ways[i].first, to_from_ways[(int) size1 - 1 - i].first);
        EXPECT_NE(from_to_ways[i].second, to_from_ways[(int) size1 - 1 - i].second);
    }
}

TEST_F(IsraelMapTest, niceRoute) {
    Coordinates origin(Longitude(32.50365),
                       Latitude(35.06183)); // J1026, closestWayPoint is on a highway (less then 3 meters away)
    Coordinates destination(Longitude(32.11181), Latitude(34.79474)); // on W2041, between J1004 and J1014
    auto routes = navigation.getRoutes(origin, destination);

    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestTime()));

    auto shortestDistanceWays = getWaysIds(routes->shortestDistance());
    auto id = std::find(shortestDistanceWays.begin(), shortestDistanceWays.end(), EntityId("W2045"));
    // shortestDistanceRoute runs through W2045
    EXPECT_NE(id, shortestDistanceWays.end());
    auto shortestTimeWays = getWaysIds(routes->shortestTime());
    id = std::find(shortestTimeWays.begin(), shortestTimeWays.end(), EntityId("W2045"));
    // shortestTimeRoute does not run through W2045
    EXPECT_EQ(id, shortestTimeWays.end());
}

/**
 * Starts from a point on a highway - the closest way is W2017 which is a highway
 */
TEST_F(IsraelMapTest, onHighway) {
    Coordinates origin(Longitude(32.50412), Latitude(35.06188)); // on a highway W2017
    Coordinates destination(Longitude(32.057), Latitude(34.86717)); // on W2023, between J1022 and J1023
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestTime()));
    auto distRouteSize = routes->shortestDistance().getWays().size();
    auto timeRouteSize = routes->shortestTime().getWays().size();
    EXPECT_NE(distRouteSize, timeRouteSize); // routes should be different according to the map
    EXPECT_EQ(routes->shortestDistance().getWays().front().first, EntityId("W2017")); //highway
    EXPECT_EQ(routes->shortestTime().getWays().front().first, EntityId("W2017")); //highway
}

/**
 * Closest Way is W2017 highway but is too far so the expected first way id is not W2017
 */
TEST_F(IsraelMapTest, highwayTooFar) {
    Coordinates origin(Longitude(32.4618), Latitude(35.08074)); // highway W2017 is the closest but too far away
    Coordinates destination(Longitude(32.057), Latitude(34.86717)); // on W2023, between J1022 and J1023
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestTime()));
    EXPECT_NE(routes->shortestDistance().getWays().front().first, EntityId("W2017")); //highway too far
    EXPECT_NE(routes->shortestTime().getWays().front().first, EntityId("W2017")); //highway too far
    auto shortestDistanceWays = getWaysIds(routes->shortestDistance());
    auto id = std::find(shortestDistanceWays.begin(), shortestDistanceWays.end(), EntityId("W2045"));
    // shortestDistanceRoute runs through W2045
    EXPECT_NE(id, shortestDistanceWays.end());
    auto shortestTimeWays = getWaysIds(routes->shortestTime());
    id = std::find(shortestTimeWays.begin(), shortestTimeWays.end(), EntityId("W2045"));
    // shortestTimeRoute does not run through W2045
    EXPECT_EQ(id, shortestTimeWays.end());
}


TEST_F(IsraelMapTest, differentRoutes) {
    Coordinates origin(Longitude(32.50365),
                       Latitude(35.06183)); // near J1026, closestWayPoint is on a highway (less then 3 meters away)
    Coordinates destination(Longitude(32.10885), Latitude(34.85451)); // J1020

    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestTime()));
    auto shortestDistanceWays = getWaysIds(routes->shortestDistance());
    auto id = std::find(shortestDistanceWays.begin(), shortestDistanceWays.end(), EntityId("W2045"));
    // shortestDistanceRoute runs through W2045
    EXPECT_NE(id, shortestDistanceWays.end());
    id = std::find(shortestDistanceWays.begin(), shortestDistanceWays.end(), EntityId("W2018"));
    // shortestDistanceRoute does not run through W2018
    EXPECT_EQ(id, shortestDistanceWays.end());

    auto shortestTimeWays = getWaysIds(routes->shortestTime());
    id = std::find(shortestTimeWays.begin(), shortestTimeWays.end(), EntityId("W2045"));
    // shortestTimeRoute does not run through W2045
    EXPECT_EQ(id, shortestTimeWays.end());
    // shortestTimeRoute run through W2018
    id = std::find(shortestTimeWays.begin(), shortestTimeWays.end(), EntityId("W2018"));
    EXPECT_NE(id, shortestTimeWays.end());
}

/**
 * This test tests two opposite route - from J1020 to J1026 and vice-versa.
 * The shortest distance from J1026 to J1020 runs through unidirectional way so the routes should differ.
 * Way W2045 is the unidirectional way
 */
TEST_F(IsraelMapTest, differentRoutesOpposite) {
    Coordinates destination(Longitude(32.50365),
                            Latitude(
                                    35.06183)); // near J1026, closestWayPoint is on a highway (less then 3 meters away)
    Coordinates origin(Longitude(32.10885), Latitude(34.85451)); // J1020
    // from J1026 to J1020:
    auto routes = navigation.getRoutes(destination, origin);
    EXPECT_TRUE(validator.validateRoute(destination, origin, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(destination, origin, routes->shortestTime()));
    auto shortestDistanceWays = getWaysIds(routes->shortestDistance());
    auto id = std::find(shortestDistanceWays.begin(), shortestDistanceWays.end(), EntityId("W2045"));
    EXPECT_NE(id, shortestDistanceWays.end());

    // from J1020 to J1026:
    auto reverseRoute = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(reverseRoute->isValid());
    shortestDistanceWays = getWaysIds(reverseRoute->shortestDistance());
    auto invalidId = std::find(shortestDistanceWays.begin(), shortestDistanceWays.end(), EntityId("W2045"));
    // The reverse route from J2020 to J1026 is not the same because there is a unidirectional way.
    EXPECT_EQ(invalidId, shortestDistanceWays.end());
}

/**
 * Unreachble way test - expected to find invalid route
 */
TEST_F(IsraelMapTest, invalidRoutesUnreachable) {
    Coordinates origin(Longitude(32.31719),
                       Latitude(35.18944)); // point on W2047 which is isolated
    Coordinates destination(Longitude(32.10885), Latitude(34.85451)); // J1020
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_FALSE(routes->isValid());
    EXPECT_EQ(routes->getErrorMessage(), "Routes not found!");
}

/**
 * First and final way are the same way which is unidirectional
 */
TEST_F(IsraelMapTest, unidirectionalSingleWay) {
//    W2047 is unidirectional
    Coordinates origin(Longitude(32.31719),
                       Latitude(35.18944)); // point on W2047 which is isolated
    Coordinates destination(Longitude(32.33931), Latitude(35.19085)); // J1033 which is on W2047
    auto routes = navigation.getRoutes(origin, destination);
    // this direction suppose to be invalid
    EXPECT_FALSE(validator.validateRoute(origin, destination, routes->shortestDistance()));
    EXPECT_FALSE(validator.validateRoute(origin, destination, routes->shortestTime()));
    EXPECT_EQ(routes->getErrorMessage(), "Routes not found!");

    // the opposite direction should work
    routes = navigation.getRoutes(destination, origin);
    EXPECT_TRUE(validator.validateRoute(destination, origin, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(destination, origin, routes->shortestTime()));
}


/**
 * First and final way are the same way which is bidirectional - Expected to find valid route
 */
TEST_F(IsraelMapTest, bidirectionalSingleWay) {
    Coordinates destination(Longitude(32.34981),
                            Latitude(35.22814)); // J1038
    Coordinates origin(Longitude(32.25985), Latitude(35.22334)); // J1039
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestTime()));
}

/**
 * Start way and final way are both W2052 which is bidirectional and also very slow.
 * The shortest time route should include another way.
 * The shortest distance route should be different from the shortest time route.
 */
TEST_F(IsraelMapTest, singleSlowWayVsFastWay) {
    Coordinates origin(Longitude(32.49647),
                       Latitude(35.03114)); // near J1026 on W2052
    Coordinates destination(Longitude(32.37961), Latitude(34.97144)); // J1029
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestTime()));
    auto &shortestDistanceRoute = routes->shortestDistance();
    auto &shortestTimeRoute = routes->shortestTime();
    EXPECT_LT(shortestDistanceRoute.totalLength(), shortestTimeRoute.totalLength());
    EXPECT_LT(shortestTimeRoute.estimatedDuration(), shortestDistanceRoute.estimatedDuration());
}
/**
 * This test tests that the route found is indeed the shortest route when the final way is bidirectional.
 * The final Way is W2050 and connect the junctions J1035 and J1036
 * the origin point is J1037 which is connected to both J1035 and J1036 but is closer to J1035
 * The destination point is placed on W2050 very close to J1036
 * The expected Route is J1037-->J1036-->destination.
 */
TEST_F(IsraelMapTest, finalBidirectionalWay) {
    Coordinates origin(Longitude(32.15044), Latitude(34.85382)); // origin is J1037
    Coordinates destination(Longitude(32.18378), Latitude(34.82216)); // near J1036, on way W2050
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestTime()));
    auto distRouteSize = routes->shortestDistance().getWays().size();
    auto timeRouteSize = routes->shortestTime().getWays().size();
    EXPECT_EQ(distRouteSize, (size_t) 2);
    EXPECT_EQ(timeRouteSize, (size_t) 2);
    auto ways = routes->shortestDistance().getWays();
    EXPECT_EQ(ways.front().first, EntityId("W2049"));
    EXPECT_EQ(ways.back().first, EntityId("W2050"));
}

/**
 * Tests to parallel ways with different speed:
 * W2054 (speed limit = 100) is parallel to W2055 (speed limit = 20)
 * Both ways are connected to W2056
 * Expected shortest distance route should run on W2054 because it is faster: W2056 --> W2054
 */
TEST_F(IsraelMapTest, parallelRoutes) {
    Coordinates origin(Longitude(32.37458), Latitude(35.13005)); // origin is J1042
    Coordinates destination(Longitude(32.51105), Latitude(35.20316)); // destination is J1040
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestTime()));
    const AbstractWay &W2054 = gis.getWay(EntityId("W2054"));
    const AbstractWay &W2055 = gis.getWay(EntityId("W2055"));
    const AbstractWay &W2056 = gis.getWay(EntityId("W2056"));
    Minutes expectedTime = Utils::calculateTime(W2054.getLength(), W2054.getSpeedLimit()) +
                           Utils::calculateTime(W2056.getLength(), W2056.getSpeedLimit());
    EXPECT_EQ(expectedTime, routes->shortestDistance().estimatedDuration());
    Minutes notExpectedTime = Utils::calculateTime(W2055.getLength(), W2055.getSpeedLimit()) +
                              Utils::calculateTime(W2056.getLength(), W2056.getSpeedLimit());
    EXPECT_NE(notExpectedTime, routes->shortestDistance().estimatedDuration());
}


/**
 * Two parallel routes (same speed limits and length)
 * 1. W2060 -> W2051
 * 2. W2059 -> W2058 -> W2057 -> W2051
 * Expected Routes result should be the first one
 */
TEST_F(IsraelMapTest, minimalWaysRoute) {
    Coordinates destination(Longitude(32.34981), Latitude(35.22814)); // origin is J1045
    Coordinates origin(Longitude(32.24811), Latitude(35.34327)); // destination is J1038
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(origin, destination, routes->shortestTime()));
    EXPECT_LT(routes->shortestDistance().getWays().size(), (size_t) 4);
}


/**
 * startWay is also the finalWay (W2122) but it is vary slow way and very long.
 * The fastest and shortest route is not to take the long way and go through a faster way in between.
 * The expected route should from origin to destination: W2122 -> W2123 -> W2122
 * The expected route should from destination to origin: W2122
 * W2122 is bidirectional
 * W2123 is unidirectional
 */

TEST_F(IsraelMapTest, accurateDistancesTest1) {
    Coordinates origin(Longitude(32.494), Latitude(35.35336));
    Coordinates destination(Longitude(32.44119), Latitude(35.30997));
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_EQ(routes->shortestDistance().getWays().size(), 3);
    EXPECT_EQ(routes->shortestTime().getWays().size(), (size_t) 3);

    // the opposite direction should include only one way because W2123 is unidirectional

    routes = navigation.getRoutes(destination, origin);
    EXPECT_EQ(routes->shortestDistance().getWays().size(), (size_t) 1);
}


/**
 * startWay is also the finalWay (W2123) but it is vary slow way and very long
 * The only route from origin to destination is W2123 -> W2122 -> W2123
 * The expected route should from destination to origin: W2123
 * W2122 is bidirectional
 * W2123 is unidirectional
 */
TEST_F(IsraelMapTest, accurateDistancesTest2) {
    Coordinates origin(Longitude(32.44504), Latitude(35.30994));
    Coordinates destination(Longitude(32.44508), Latitude(35.30997));
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_EQ(routes->shortestDistance().getWays().size(), 3);
    EXPECT_EQ(routes->shortestTime().getWays().size(), 3);

    // the opposite direction should include only one way
    routes = navigation.getRoutes(destination, origin);
    EXPECT_EQ(routes->shortestDistance().getWays().size(), (size_t) 1);

}

TEST_F(IsraelMapTest, accurateDistancesTest3) {
    Coordinates origin(Longitude(32.44121), Latitude(35.30718)); //J1072
    Coordinates destination(Longitude(32.49779), Latitude(35.34791)); //J1071
    auto routes = navigation.getRoutes(origin, destination);
    printRoutes(*routes);
//    EXPECT_EQ(routes->shortestDistance().getWays().size(), 3);
//    EXPECT_EQ(routes->shortestTime().getWays().size(), 3);
}
