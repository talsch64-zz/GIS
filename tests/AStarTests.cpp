
#include "gtest/gtest.h"
#include "../GISNamedTypes.h"
#include "../GISProvider.h"
#include "../CoordinatesMath.h"
#include "../navigation/Navigation.h"
#include "../entities/Way.h"

TEST(GISBasic, routeToFrom) {
    GIS gis = GISProvider::getGIS();
    NavigationGIS navGis(gis);
    Navigation nav(navGis);
    Coordinates from(Longitude(32.113357), Latitude(34.801290));
    Coordinates to(Longitude(32.111320), Latitude(34.796037));
    gis.loadMapFile("route_test.json");
    auto routes = nav.getRoutes(from, to);
    auto size = routes.shortestDistance().getWays().size();
//    TODO delete
//    for (const auto &way: routes.shortestDistance().getWays()) {
//        std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
//                  << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
//        std::cout << "length: " << (double) navGis.getWay(way.first).getLength() << ", time: "
//                  << (double) navGis.getWay(way.first).getTime() << std::endl;
//        std::cout << "==================================" << std::endl;
//    }
//    std::cout << "shortestDistanceRoute: distance: " << (double) routes.shortestDistance().totalLength() << ", time: "
//              << (double) routes.shortestDistance().estimatedDuration() << std::endl;
//    std::cout << "shortestTimeRoute: distance: " << (double) routes.shortestTime().totalLength() << ", time: "
//              << (double) routes.shortestTime().estimatedDuration() << std::endl;
    EXPECT_EQ(size, 3);
}


TEST(GISBasic, routeFromTo) {
    GIS gis = GISProvider::getGIS();
    NavigationGIS navGis(gis);
    Navigation nav(navGis);
    Coordinates to(Longitude(32.113357), Latitude(34.801290));
    Coordinates from(Longitude(32.11181), Latitude(34.79474));
    gis.loadMapFile("route_test.json");
    auto routes = nav.getRoutes(from, to);
    auto size = routes.shortestDistance().getWays().size();
    //TODO delete!
//    for (const auto &way : routes.shortestDistance().getWays()) {
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
//              << (double) routes.shortestTime().estimatedDuration() << std::endl;
    EXPECT_EQ(size, 3);
}

TEST(GISBasic, niceRoute) {
    GIS gis = GISProvider::getGIS();
    NavigationGIS navGis(gis);
    Navigation nav(navGis);
    Coordinates origin(Longitude(32.50365), Latitude(35.06183)); // J1026, closestWayPoint is on a highway (less then 3 meters away)
    Coordinates destination(Longitude(32.11181), Latitude(34.79474)); // on W2041, between J1004 and J1014
    gis.loadMapFile("israel.json");
    auto routes = nav.getRoutes(origin, destination);
    auto size = routes.shortestDistance().getWays().size();
    for (const auto &way : routes.shortestDistance().getWays()) {
        std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
                  << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
        std::cout << "length: " << (double) navGis.getWay(way.first).getLength() << ", time: "
                  << (double) navGis.getWay(way.first).getTime() << std::endl;
        std::cout << "==================================" << std::endl;

    }
    std::cout << "shortestDistanceRoute: distance: " << (double) routes.shortestDistance().totalLength() << ", time: "
              << (double) routes.shortestDistance().estimatedDuration() << std::endl;
    std::cout << "shortestTimeRoute: distance: " << (double) routes.shortestTime().totalLength() << ", time: "
              << (double) routes.shortestTime().estimatedDuration() << std::endl << std::endl;
    EXPECT_EQ(size, 8);
}

TEST(GISBasic, niceRoute2) {
    GIS gis = GISProvider::getGIS();
    NavigationGIS navGis(gis);
    Navigation nav(navGis);
    Coordinates origin(Longitude(32.50428), Latitude(35.06188)); // near J1026
    Coordinates destination(Longitude(32.057), Latitude(34.86717)); // on W2023, between J1022 and J1023
    gis.loadMapFile("israel.json");
    auto routes = nav.getRoutes(origin, destination);
    auto size = routes.shortestDistance().getWays().size();
    for (const auto &way : routes.shortestDistance().getWays()) {
        std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
                  << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
        std::cout << "length: " << (double) navGis.getWay(way.first).getLength() << ", time: "
                  << (double) navGis.getWay(way.first).getTime() << std::endl;
        std::cout << "==================================" << std::endl;

    }
    std::cout << "shortestDistanceRoute: distance: " << (double) routes.shortestDistance().totalLength() << ", time: "
              << (double) routes.shortestDistance().estimatedDuration() << std::endl;
    std::cout << "shortestTimeRoute: distance: " << (double) routes.shortestTime().totalLength() << ", time: "
              << (double) routes.shortestTime().estimatedDuration() << std::endl << std::endl;
    EXPECT_EQ(size, 6);
}