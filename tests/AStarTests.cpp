
#include "gtest/gtest.h"
#include "../GISNamedTypes.h"
#include "../GISProvider.h"
#include "../CoordinatesMath.h"
#include "../navigation/Navigation.h"
#include "../entities/Way.h"

class IsraelMapTest: public ::testing::Test {
protected:
    GIS gis;
    NavigationGIS navGis;
    Navigation navigation;
public:
    IsraelMapTest(): gis(GISProvider::getGIS()), navGis(gis), navigation(gis) {
    }

    void SetUp() {
        gis.loadMapFile("israel.json");
    }

    void TearDown() {

    }
    void printRoutes(Routes routes) {
        std::cout << std::endl << "---ShortestDistance Route--- " << std::endl;

        for (const auto &way : routes.shortestDistance().getWays()) {
            std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
                      << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
            std::cout << "length: " << (double) gis.getWay(way.first).getLength() << ", time: "
                      << (double) gis.getWay(way.first).getTime() << std::endl;
            std::cout << "==================================" << std::endl;
        }

        std::cout << std::endl << "---ShortestTime Route--- " << std::endl;

        for (const auto &way : routes.shortestTime().getWays()) {
            std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
                      << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
            std::cout << "length: " << (double) gis.getWay(way.first).getLength() << ", time: "
                      << (double) gis.getWay(way.first).getTime() << std::endl;
            std::cout << "==================================" << std::endl;
        }
        std::cout << "ShortestDistanceRoute - distance: " << (double) routes.shortestDistance().totalLength() << ", time: "
                  << (double) routes.shortestDistance().estimatedDuration() << std::endl;
        std::cout << "ShortestTimeRoute - distance: " << (double) routes.shortestTime().totalLength() << ", time: "
                  << (double) routes.shortestTime().estimatedDuration() << std::endl;
    }
};


TEST_F(IsraelMapTest, routeToFrom) {
    Coordinates from(Longitude(32.113357), Latitude(34.801290)); // J1001
    Coordinates to(Longitude(32.11265), Latitude(34.79254)); // J1014
    auto routes = navigation.getRoutes(from, to);
    auto size = routes.shortestDistance().getWays().size();
    EXPECT_EQ(size, 5);
}


TEST_F(IsraelMapTest, routeFromTo) {
    Coordinates to(Longitude(32.113357), Latitude(34.801290)); // J1001
    Coordinates from(Longitude(32.11265), Latitude(34.79254)); // J1014
    auto routes = navigation.getRoutes(from, to);
    auto size = routes.shortestDistance().getWays().size();
    EXPECT_EQ(size, 5);
}

TEST_F(IsraelMapTest, niceRoute) {
    Coordinates origin(Longitude(32.50365),
                       Latitude(35.06183)); // J1026, closestWayPoint is on a highway (less then 3 meters away)
    Coordinates destination(Longitude(32.11181), Latitude(34.79474)); // on W2041, between J1004 and J1014
    auto routes = navigation.getRoutes(origin, destination);
    auto distRouteSize = routes.shortestDistance().getWays().size();
    auto timeRouteSize = routes.shortestTime().getWays().size();
    EXPECT_EQ(distRouteSize, 11);
    EXPECT_EQ(timeRouteSize, 10);
}

TEST_F(IsraelMapTest, onHighway) {
    Coordinates origin(Longitude(32.50428), Latitude(35.06188)); // on a highway
    Coordinates destination(Longitude(32.057), Latitude(34.86717)); // on W2023, between J1022 and J1023
    auto routes = navigation.getRoutes(origin, destination);
    auto distRouteSize = routes.shortestDistance().getWays().size();
    auto timeRouteSize = routes.shortestTime().getWays().size();
    EXPECT_EQ(distRouteSize, 9);
    EXPECT_EQ(timeRouteSize, 6);
//    printRoutes(routes);


}


TEST_F(IsraelMapTest, highwayTooFar) {
    Coordinates origin(Longitude(32.4618), Latitude(35.08074)); // highway W2017 is the closest but too far away
    Coordinates destination(Longitude(32.057), Latitude(34.86717)); // on W2023, between J1022 and J1023
    auto routes = navigation.getRoutes(origin, destination);
    auto distRouteSize = routes.shortestDistance().getWays().size();
    auto timeRouteSize = routes.shortestTime().getWays().size();
    EXPECT_EQ(distRouteSize, 8);
    EXPECT_EQ(timeRouteSize, 7);
    printRoutes(routes);

}


TEST_F(IsraelMapTest, differentRoutes) {
    Coordinates origin(Longitude(32.50365),
                       Latitude(35.06183)); // near J1026, closestWayPoint is on a highway (less then 3 meters away)
    Coordinates destination(Longitude(32.10885), Latitude(34.85451)); // J2020

    auto routes = navigation.getRoutes(origin, destination);
    auto sizeTime = routes.shortestTime().getWays().size();
    auto sizeDistance = routes.shortestDistance().getWays().size();
    auto distRouteSize = routes.shortestDistance().getWays().size();
    auto timeRouteSize = routes.shortestTime().getWays().size();
    EXPECT_EQ(distRouteSize, 7);
    EXPECT_EQ(timeRouteSize, 5);

}



//TEST(GISBasic, routeToFrom) {
//    GIS gis = GISProvider::getGIS();
//    NavigationGIS navGis(gis);
//    Navigation nav(navGis);
//    Coordinates from(Longitude(32.113357), Latitude(34.801290));
//    Coordinates to(Longitude(32.111320), Latitude(34.796037));
//    gis.loadMapFile("israel.json");
//    auto routes = nav.getRoutes(from, to);
//    auto size = routes.shortestDistance().getWays().size();
//    EXPECT_EQ(size, 3);
//}
//
//
//TEST(GISBasic, routeFromTo) {
//    GIS gis = GISProvider::getGIS();
//    NavigationGIS navGis(gis);
//    Navigation nav(navGis);
//    Coordinates to(Longitude(32.113357), Latitude(34.801290));
//    Coordinates from(Longitude(32.11181), Latitude(34.79474));
//    gis.loadMapFile("route_test.json");
//    auto routes = nav.getRoutes(from, to);
//    auto size = routes.shortestDistance().getWays().size();
//    EXPECT_EQ(size, 3);
//}
//
//TEST(GISBasic, niceRoute) {
//    GIS gis = GISProvider::getGIS();
//    NavigationGIS navGis(gis);
//    Navigation nav(navGis);
//    Coordinates origin(Longitude(32.50365),
//                       Latitude(35.06183)); // J1026, closestWayPoint is on a highway (less then 3 meters away)
//    Coordinates destination(Longitude(32.11181), Latitude(34.79474)); // on W2041, between J1004 and J1014
//    gis.loadMapFile("israel.json");
//    auto routes = nav.getRoutes(origin, destination);
//    auto size = routes.shortestDistance().getWays().size();
//    std::cout << "============================== test 3 ==============================" << std::endl << std::endl;
//    std::cout << "============================== Shortest Distance: ==============================" << std::endl
//              << std::endl;
//    for (const auto &way : routes.shortestDistance().getWays()) {
//        std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
//                  << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
//        std::cout << "length: " << (double) navGis.getWay(way.first).getLength() << ", time: "
//                  << (double) navGis.getWay(way.first).getTime() << std::endl;
//        std::cout << "==================================" << std::endl;
//
//    }
//
//    std::cout << "--------------------------------------------------------" << std::endl;
//    std::cout << "============================== Shortest Time: ==============================" << std::endl
//              << std::endl;
//
//    for (const auto &way : routes.shortestTime().getWays()) {
//        std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
//                  << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
//        std::cout << "length: " << (double) navGis.getWay(way.first).getLength() << ", time: "
//                  << (double) navGis.getWay(way.first).getTime() << std::endl;
//        std::cout << "==================================" << std::endl;
//
//    }
//    std::cout << "shortestDistanceRoute: distance: " << (double) routes.shortestDistance().totalLength() << ", time: "
//              << (double) routes.shortestDistance().estimatedDuration() << std::endl;
//    std::cout << "shortestTimeRoute: distance: " << (double) routes.shortestTime().totalLength() << ", time: "
//              << (double) routes.shortestTime().estimatedDuration() << std::endl << std::endl;
//    EXPECT_EQ(size, 8);
//}
//
//TEST(GISBasic, onHighway) {
//    GIS gis = GISProvider::getGIS();
//    NavigationGIS navGis(gis);
//    Navigation nav(navGis);
//    Coordinates origin(Longitude(32.50428), Latitude(35.06188)); // on a highway
//    Coordinates destination(Longitude(32.057), Latitude(34.86717)); // on W2023, between J1022 and J1023
//    gis.loadMapFile("israel.json");
//    auto routes = nav.getRoutes(origin, destination);
//    auto size = routes.shortestDistance().getWays().size();
//    EXPECT_EQ(std::string(routes.shortestDistance().getWays().begin()->first), "W2017");
//    EXPECT_EQ(size, 6);
//}
//
//
//TEST(GISBasic, highwayTooFar) {
//    GIS gis = GISProvider::getGIS();
//    NavigationGIS navGis(gis);
//    Navigation nav(navGis);
//    Coordinates origin(Longitude(32.4618), Latitude(35.08074)); // highway is the closest but too far away
//    Coordinates destination(Longitude(32.057), Latitude(34.86717)); // on W2023, between J1022 and J1023
//    gis.loadMapFile("israel.json");
//    auto routes = nav.getRoutes(origin, destination);
//    auto size = routes.shortestDistance().getWays().size();
//    EXPECT_EQ(std::string(routes.shortestDistance().getWays().begin()->first), "W2036"); // not an highway
//    EXPECT_EQ(size, 6);
//}
//
//
//TEST(GISBasic, differentRoutes) {
//    GIS gis = GISProvider::getGIS();
//    NavigationGIS navGis(gis);
//    Navigation nav(navGis);
//    Coordinates origin(Longitude(32.50365),
//                       Latitude(35.06183)); // J1026, closestWayPoint is on a highway (less then 3 meters away)
//    Coordinates destination(Longitude(32.10885), Latitude(34.85451)); // J2020
//    gis.loadMapFile("israel.json");
//    auto routes = nav.getRoutes(origin, destination);
//    auto size = routes.shortestDistance().getWays().size();
//    EXPECT_EQ(size, 6);
//}


