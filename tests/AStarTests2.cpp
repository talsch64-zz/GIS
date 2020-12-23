#include "gtest/gtest.h"
#include "GISMock.h"
#include "../navigation/Route.h"
#include "../NavigationGIS.h"
#include "../navigation/Navigation.h"
#include "RandTestUtils.h"
#include "../navigation/NavigationValidator.h"

void assertRoute(const std::vector<std::pair<EntityId, Direction>> &expected, const Route &actual) {
    auto routeIterator = actual.getWays().begin();
//    EXPECT_EQ(actual.getWays().size(), expected.size());
    for (auto &wayPair : expected) {
//        EXPECT_EQ(wayPair.first, routeIterator->first);
//        EXPECT_EQ(wayPair.second, routeIterator->second);
        routeIterator++;
    }
}

void compareRoutes(const Route &actualRoute, const Route &expectedRoute) {
    EXPECT_EQ(actualRoute.isValid(), expectedRoute.isValid());
    if (actualRoute.isValid()) {
        assertRoute(expectedRoute.getWays(), actualRoute);
        EXPECT_DOUBLE_EQ((double) actualRoute.totalLength(), (double) expectedRoute.totalLength());
        EXPECT_DOUBLE_EQ((double) actualRoute.estimatedDuration(), (double) expectedRoute.estimatedDuration());
//        EXPECT_EQ(actualRoute.getWayStartPoint(), expectedRoute.getWayStartPoint());
//        EXPECT_EQ(actualRoute.getWayEndPoint(), expectedRoute.getWayEndPoint());
    }
}

void compareRoutes(const Routes &actualRoute, const Routes &expectedRoute) {
    EXPECT_EQ(actualRoute.isValid(), expectedRoute.isValid());
    if (actualRoute.isValid()) {
        compareRoutes(actualRoute.shortestTime(), expectedRoute.shortestTime());
        compareRoutes(actualRoute.shortestDistance(), expectedRoute.shortestDistance());
    }
}

TEST(AStar, BasicPathTest) {
    GISMock gis;
    gis.loadMapFile("astar.json");
    NavigationGIS navigationGis(gis);
    Navigation navigation(navigationGis);
    NavigationValidator navigationValidator(gis);
    std::vector<std::pair<EntityId, Direction>> expectedDistanceRoute = {
            std::make_pair(EntityId("way1"), Direction::A_to_B),
            std::make_pair(EntityId("way2"), Direction::A_to_B),
            std::make_pair(EntityId("way5"), Direction::A_to_B),
            std::make_pair(EntityId("way6"), Direction::A_to_B)
    };
    Coordinates startCoord(Longitude(22.57676323126267),
                           Latitude(-30.64052973777345));
    Coordinates endCoord(Longitude(22.273619),
                         Latitude(-30.497604));

    Routes routes = navigation.getRoutes(startCoord, endCoord);
    const Route &shortestDistance = routes.shortestDistance();
    const Route &shortestTime = routes.shortestTime();
    assertRoute(expectedDistanceRoute, shortestDistance);
    assertRoute(expectedDistanceRoute, shortestTime);
    EXPECT_TRUE(navigationValidator.validateRoute(startCoord, endCoord, shortestDistance));
    EXPECT_TRUE(navigationValidator.validateRoute(startCoord, endCoord, shortestTime));
}

TEST(AStar, WrongPathsAppearCloserToTarget) {
    GIS gis;
    gis.loadMapFile("astar2.json");
    NavigationGIS navigationGis(gis);
    Navigation navigation(navigationGis);
    NavigationValidator navigationValidator(gis);
    std::vector<std::pair<EntityId, Direction>> expectedDistanceRoute = {
            std::make_pair(EntityId("way4"), Direction::B_to_A),
            std::make_pair(EntityId("way2"), Direction::A_to_B),
            std::make_pair(EntityId("way3"), Direction::A_to_B),
            std::make_pair(EntityId("way9"), Direction::A_to_B),
            std::make_pair(EntityId("way10"), Direction::A_to_B)
    };
    Coordinates startCoord(Longitude(32.85226656648769),
                           Latitude(39.89576931048837));
    Coordinates endCoord(Longitude(32.81894402639684),
                         Latitude(39.92570973912446));

    Routes routes = navigation.getRoutes(startCoord, endCoord);
    const Route &shortestDistance = routes.shortestDistance();
    const Route &shortestTime = routes.shortestTime();
    assertRoute(expectedDistanceRoute, shortestDistance);
    assertRoute(expectedDistanceRoute, shortestTime);
    EXPECT_TRUE(navigationValidator.validateRoute(startCoord, endCoord, shortestDistance));
    EXPECT_TRUE(navigationValidator.validateRoute(startCoord, endCoord, shortestTime));
}

TEST(AStar, InvalidRouteLastWayHighway) {
    GIS gis;
    gis.loadMapFile("astar3.json");
    NavigationGIS navigationGis(gis);
    Navigation navigation(navigationGis);
    Coordinates startCoord(Longitude(20.86616914522187),
                           Latitude(43.99225503398053));
    Coordinates endCoord(Longitude(20.88795928805733),
                         Latitude(43.99705953606207));

    Routes routes = navigation.getRoutes(startCoord, endCoord);
    EXPECT_FALSE(routes.isValid());
}

TEST(AStar, HighwayTooFar) {
    GIS gis;
    gis.loadMapFile("astar3b.json");
    NavigationGIS navigationGis(gis);
    Navigation navigation(navigationGis);
    NavigationValidator navigationValidator(gis);
    std::vector<std::pair<EntityId, Direction>> expectedDistanceRoute = {
            std::make_pair(EntityId("way3"), Direction::B_to_A),
            std::make_pair(EntityId("way2"), Direction::A_to_B)
    };
    Coordinates startCoord(Longitude(20.86616914522187),
                           Latitude(43.99225503398053));
    Coordinates endCoord(Longitude(20.88795928805733),
                         Latitude(43.99705953606207));

    Routes routes = navigation.getRoutes(startCoord, endCoord);
    const Route &shortestDistance = routes.shortestDistance();
    const Route &shortestTime = routes.shortestTime();
    assertRoute(expectedDistanceRoute, shortestDistance);
    assertRoute(expectedDistanceRoute, shortestTime);
    EXPECT_TRUE(navigationValidator.validateRoute(startCoord, endCoord, shortestDistance));
    EXPECT_TRUE(navigationValidator.validateRoute(startCoord, endCoord, shortestTime));
}

TEST(AStar, HighwayWithinThreeMeters) {
    GIS gis;
    gis.loadMapFile("astar3b.json");
    NavigationGIS navigationGis(gis);
    Navigation navigation(navigationGis);
    NavigationValidator navigationValidator(gis);
    std::vector<std::pair<EntityId, Direction>> expectedDistanceRoute = {
            std::make_pair(EntityId("way1"), Direction::A_to_B),
            std::make_pair(EntityId("way2"), Direction::B_to_A)
    };
    Coordinates startCoord(Longitude(20.86452069048904),
                           Latitude(43.992280969386));
    Coordinates endCoord(Longitude(20.88795928805733),
                         Latitude(43.99705953606207));

    Routes routes = navigation.getRoutes(startCoord, endCoord);

    const Route &shortestDistance = routes.shortestDistance();
    const Route &shortestTime = routes.shortestTime();
    assertRoute(expectedDistanceRoute, shortestDistance);
    assertRoute(expectedDistanceRoute, shortestTime);
    EXPECT_TRUE(navigationValidator.validateRoute(startCoord, endCoord, shortestDistance));
    EXPECT_TRUE(navigationValidator.validateRoute(startCoord, endCoord, shortestTime));
}

/**
 * Beware! Infinite Improbability Drive may cause unexpected results!
 */
TEST(AStar, HeartOfGold) {
    srand(56);
    std::unique_ptr<GISMock> gis = std::make_unique<GISMock>();
    IdGenerator idGenerator;
    int v = 8, e = 14, reps = 50;
    for (int i = 0; i < reps; i++) {
        Bound bound = RandTestUtils::randBound();
        auto junctions = RandTestUtils::generateJunctions(*gis, idGenerator, v, bound);
        RandTestUtils::generateWays(*gis, idGenerator, e, bound, junctions);
        std::cout << "rep" << i << std::endl;
        gis->saveMapFile("HeartOfGoldTest_rep" + std::to_string(i) + ".json");

        Coordinates startCoord = RandTestUtils::randCoord(bound);
        Coordinates endCoord = RandTestUtils::randCoord(bound);
        NavigationGIS navigationGis(*gis);
        Navigation navigation(navigationGis);

        Routes routes = navigation.getRoutes(startCoord, endCoord);
        Routes routesExpected = RandTestUtils::getBestRoutes(*gis, idGenerator, startCoord, endCoord);

        compareRoutes(routes, routesExpected);
        gis->clear();
    }
}
