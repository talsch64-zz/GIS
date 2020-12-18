
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
    Coordinates from(Longitude(34.801290), Latitude(32.113357));
    Coordinates to(Longitude(34.796037), Latitude(32.111320));
    gis.loadMapFile("route_test.json");
    auto routes = nav.getRoutes(from, to);
    auto size = routes.shortestDistance().getWays().size();
    for (const auto &way: routes.shortestDistance().getWays()) {
        std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
                  << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
        std::cout << "length: " << (double) navGis.getWay(way.first).getLength() << ", time: "
                  << (double) navGis.getWay(way.first).getTime() << std::endl;
        std::cout << "==================================" << std::endl;
    }
    std::cout << "shortestDistanceRoute: distance: " << (double) routes.shortestDistance().totalLength() << ", time: "
              << (double) routes.shortestDistance().estimatedDuration() << std::endl;
    std::cout << "shortestTimeRoute: distance: " << (double) routes.shortestTime().totalLength() << ", time: "
              << (double) routes.shortestTime().estimatedDuration() << std::endl;
    EXPECT_EQ(size, 3);
}


TEST(GISBasic, routeFromTo) {
    GIS gis = GISProvider::getGIS();
    NavigationGIS navGis(gis);
    Navigation nav(navGis);
    Coordinates to(Longitude(34.801290), Latitude(32.113357));
    Coordinates from(Longitude(34.796037), Latitude(32.111320));
    gis.loadMapFile("route_test.json");
    auto routes = nav.getRoutes(from, to);
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
              << (double) routes.shortestTime().estimatedDuration() << std::endl;
    EXPECT_EQ(size, 3);
}
