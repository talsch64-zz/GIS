//#include "gtest/gtest.h"

#include <iostream>
#include "GIS.h"
#include "navigation/Navigation.h"
#include "GISProvider.h"
#include "CoordinatesMath.h"

int main(int argc, char **argv) {
    Coordinates from(Longitude(34.801290), Latitude(32.113357));
    Coordinates to(Longitude(34.796037), Latitude(32.111320));

    Coordinates endTo(Longitude(34.796037), Latitude(32.111320));
    Coordinates endFrom(Longitude(34.801290), Latitude(32.113357));


    double distance1 = (double)CoordinatesMath::calculateDistance(to, endTo);
    double distance2 = (double)CoordinatesMath::calculateDistance(from, endFrom);

    std::cout << distance1 << std::endl;
    std::cout << distance2 << std::endl;




    return 0;
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
}
