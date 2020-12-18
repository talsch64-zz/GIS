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
    NavigationGIS navigationGIS(gis);
    Navigation navigator(navigationGIS);
    Routes routes = navigator.getRoutes(from, to);
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "shortestDistanceRoute: distance: " << (double) routes.shortestDistance().totalLength() << ", time: " << (double) routes.shortestDistance().estimatedDuration() << std::endl;
    std::cout << "shortestTimeRoute: distance: " << (double) routes.shortestTime().totalLength() << ", time: " << (double) routes.shortestTime().estimatedDuration() << std::endl;



    return 0;
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
}
