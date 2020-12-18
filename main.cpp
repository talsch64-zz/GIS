//#include "gtest/gtest.h"

#include <iostream>
#include "GIS.h"
#include "navigation/Navigation.h"
#include "GISProvider.h"

int main(int argc, char **argv) {
    GIS gis;
    gis.loadMapFile("/home/student/Desktop/advanced-ex1/route_test.json");
    Coordinates to(Longitude(34.801290), Latitude(32.113357));
    Coordinates from(Longitude(34.797991), Latitude(32.1115927));


    auto toPair = gis.getWayClosestPoint(to);
    auto fromPair = gis.getWayClosestPoint(from);
    std::cout << "to closest point - ";
    std::cout << "longitude: " << (double) toPair.first.longitude() << ", latitude: " << (double) toPair.first.latitude() << std::endl;
    std::cout << "from closest point - ";
    std::cout << "longitude: " << (double) fromPair.first.longitude() << ", latitude: " << (double) fromPair.first.latitude() << std::endl;

    std::cout << "to closest way " << (std::string) toPair.second << std::endl;
    std::cout << "from closest way " << (std::string) fromPair.second << std::endl;

    NavigationGIS navigationGIS(gis);
    Navigation navigator(navigationGIS);
    Routes routes = navigator.getRoutes(from, to);

//    GIS gis = GISProvider::getGIS();
//    NavigationGIS navGis(gis);
//    Navigation nav(navGis);
//    Coordinates from(Longitude(34.801290), Latitude(32.113357));
//    Coordinates to(Longitude(34.796037), Latitude(32.111320));
//    gis.loadMapFile("/home/student/Desktop/advanced-ex1/route_test.json");
//    auto routes = nav.getRoutes(from, to);
//    auto size = routes.shortestDistance().getWays().size();
//    for(const auto &way: routes.shortestDistance().getWays()) {
//        std::cout << "ID: " <<  static_cast<std::string>(way.first) <<  " Direction: " << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
//    }



    return 0;
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
}
