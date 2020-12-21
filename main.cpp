//#include "gtest/gtest.h"

#include <iostream>
#include "GIS.h"
#include "navigation/Navigation.h"
#include "GISProvider.h"
#include "CoordinatesMath.h"

int main(int argc, char **argv) {
    GIS gis;
    gis.loadMapFile("/home/student/Desktop/advanced-ex1/israel.json");
    gis.saveMapFile("/home/student/Desktop/advanced-ex1/output.json");
    GIS gis2;
    gis2.loadMapFile("/home/student/Desktop/advanced-ex1/output.json");

    return 0;
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
}
