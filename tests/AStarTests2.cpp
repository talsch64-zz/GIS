#include "gtest/gtest.h"
#include "GISMock.h"
#include "../navigation/Route.h"
#include "../NavigationGIS.h"
#include "../navigation/Navigation.h"
#include "RandTestUtils.h"

void assertRoute(const std::vector<std::pair<EntityId, Direction>> &expected, const Route &actual) {
    auto routeIterator = actual.getWays().begin();
    ASSERT_EQ(actual.getWays().size(), expected.size());
    for (auto &wayPair : expected) {
        ASSERT_EQ(wayPair.first, routeIterator->first);
        ASSERT_EQ(wayPair.second, routeIterator->second);
        routeIterator++;
    }
}

void compareRoutes(const Route &routeA, const Route &routeB) {
    ASSERT_EQ(routeA.isValid(), routeB.isValid());
    if (routeA.isValid()) {
        assertRoute(routeA.getWays(), routeB);
        ASSERT_DOUBLE_EQ((double) routeA.totalLength(), (double) routeB.totalLength());
        ASSERT_DOUBLE_EQ((double) routeA.estimatedDuration(), (double) routeB.estimatedDuration());
        ASSERT_EQ(routeA.getWayStartPoint(), routeB.getWayStartPoint());
        ASSERT_EQ(routeA.getWayEndPoint(), routeB.getWayEndPoint());
    }
}

void compareRoutes(const Routes &routesA, const Routes &routesB) {
    ASSERT_EQ(routesA.isValid(), routesB.isValid());
    if (routesA.isValid()) {
        compareRoutes(routesA.shortestTime(), routesB.shortestTime());
        compareRoutes(routesA.shortestDistance(), routesB.shortestDistance());
    }
}

TEST(AStar, BasicPathTest) {
    GISMock gis;
    gis.loadMapFile("astar.json");
    NavigationGIS navigationGis(gis);
    Navigation navigation(navigationGis);
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

//    Routes routes = navigation.getRoutes(startCoord, endCoord);
    IdGenerator idGenerator;
    std::unique_ptr<RouteMock> bestTimeRoute = std::make_unique<RouteMock>(startCoord, endCoord, Meters(0), Minutes(0),
                                                                           std::vector<std::pair<EntityId, Direction>>(),
                                                                           false);
    std::unique_ptr<RouteMock> bestDistanceRoute = std::make_unique<RouteMock>(startCoord, endCoord, Meters(0),
                                                                               Minutes(0),
                                                                               std::vector<std::pair<EntityId, Direction>>(),
                                                                               false);
    Routes routes = RandTestUtils::getBestRoutes(gis, idGenerator, *bestTimeRoute, *bestDistanceRoute, startCoord,
                                                 endCoord);
    const Route &shortestDistance = routes.shortestDistance();
    const Route &shortestTime = routes.shortestTime();
    assertRoute(expectedDistanceRoute, shortestDistance);
    assertRoute(expectedDistanceRoute, shortestTime);
    EXPECT_TRUE(routes.isValid());
}

TEST(AStar, WrongPathsAppearCloserToTarget) {
    GIS gis;
    gis.loadMapFile("astar2.json");
    NavigationGIS navigationGis(gis);
    Navigation navigation(navigationGis);
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
    EXPECT_TRUE(routes.isValid());
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

TEST(AStar, HighwayTooFar
) {
    GIS gis;
    gis.loadMapFile("astar3b.json");
    NavigationGIS navigationGis(gis);
    Navigation navigation(navigationGis);
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
    EXPECT_TRUE(routes.isValid());
}

TEST(AStar, HighwayWithinThreeMeters
) {
    GIS gis;
    gis.loadMapFile("astar3b.json");
    NavigationGIS navigationGis(gis);
    Navigation navigation(navigationGis);
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
    EXPECT_TRUE(routes.isValid());
}

/**
 * Beware! Infinite Improbability Drive may cause unexpected results!
 */
TEST(AStar, HeartOfGold) {
    std::unique_ptr<GISMock> gis = std::make_unique<GISMock>();
    IdGenerator idGenerator;
    int v = 10, e = 20;
    Bound bound = RandTestUtils::randBound();
    auto junctions = RandTestUtils::generateJunctions(*gis, idGenerator, v, bound);
    RandTestUtils::generateWays(*gis, idGenerator, v, bound, junctions);
    Coordinates startCoord = RandTestUtils::randCoord(bound);
    Coordinates endCoord = RandTestUtils::randCoord(bound);
    NavigationGIS navigationGis(*gis);
    Navigation navigation(navigationGis);

    Routes routes = navigation.getRoutes(startCoord, endCoord);
    std::unique_ptr<RouteMock> bestTimeRoute = std::make_unique<RouteMock>(startCoord, endCoord, Meters(0), Minutes(0),
                                                                           std::vector<std::pair<EntityId, Direction>>(),
                                                                           false);
    std::unique_ptr<RouteMock> bestDistanceRoute = std::make_unique<RouteMock>(startCoord, endCoord, Meters(0),
                                                                               Minutes(0),
                                                                               std::vector<std::pair<EntityId, Direction>>(),
                                                                               false);
    Routes routesExpected = RandTestUtils::getBestRoutes(*gis, idGenerator, *bestTimeRoute, *bestDistanceRoute,
                                                         startCoord, endCoord);

    compareRoutes(routes, routesExpected);
}
