#include <iostream>
#include "GIS.h"
#include "gtest/gtest.h"

using std::cout;
using std::endl;

int main(int argc, char **argv) {
    GIS gis;
    gis.loadMapFile("russia.json");
    Coordinates coord(Longitude(90.28674), Latitude(65.77863));
    Coordinates to(Longitude(90.28177), Latitude(65.89199));
    gis.getWayClosestPoint(coord);
    gis.saveMapFile("output.json");
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
