#include "gtest/gtest.h"
#include "GISMock.h"
#include "../navigation/Route.h"
#include "../NavigationGIS.h"
#include "../navigation/Navigation.h"

void assertRoute(const std::vector<std::pair<EntityId, Direction>> &expected, const Route &actual) {
    auto routeIterator = actual.getWays().begin();
    ASSERT_EQ(actual.getWays().size(), expected.size());
    for (auto &wayPair : expected) {
        ASSERT_EQ(wayPair.first, routeIterator->first);
        ASSERT_EQ(wayPair.second, routeIterator->second);
        routeIterator++;
    }
}

TEST(AStar, BasicPathTest) {
    GIS gis;
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

    Routes routes = navigation.getRoutes(startCoord, endCoord);
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

TEST(AStar, HighwayTooFar) {
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
}
