
#include "GIS.h"
#include "CoordinatesMath.h"
#include "Restrictions.h"

int main(int argc, char **argv) {
    Restrictions r("aa, bb cc ,");
    Restrictions r1(", , ,  ");
    Restrictions r2("");
    std::unordered_set<std::string> restrictions = r1.getRestrictions();
    restrictions = r2.getRestrictions();

    GIS gis;
    gis.loadMapFile("/home/student/Desktop/advanced-ex1/tests/russia.json");
//    gis.loadMapFile("/home/student/Desktop/advanced-ex1/tests/russia_bi.json");

//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
    return 0;
}
