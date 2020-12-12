//#include "gtest/gtest.h"

#include "GIS.h"

int main(int argc, char **argv) {
    GIS gis;
    if (argc) {
        if (*argv) {}
    }
    gis.loadMapFile("/home/student/Desktop/advanced-ex1/russia.json");
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
    return 0;
}
