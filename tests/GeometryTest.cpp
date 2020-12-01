#include "gtest/gtest.h"
#include "../GISdefs.h"
#include "../entities/geometry/CoordinatesMath.h"
#include "../entities/geometry/Circle.h"
#include "../entities/geometry/PointList.h"
#include "../GIS.h"

#define METERS_PRECISION 1
TEST(ClosestPointTest, ClosestPointTest_Way_Test1) {
    Coordinates c1(Longitude(-2), Latitude(2));
    Coordinates c2(Longitude(-2), Latitude(1));
    Coordinates c3(Longitude(-3), Latitude(0.5));
    Coordinates c4(Longitude(0), Latitude(1));
    Coordinates c5(Longitude(-3), Latitude(1.3));
    Coordinates c6(Longitude(-5), Latitude(1.8));

    Coordinates target(Longitude(-1), Latitude(4));
    std::vector<Coordinates> vector {c1, c2, c3, c4, c5, c6};
    PointList points(vector);
    Coordinates closest = points.getClosestPoint(target);

    EXPECT_TRUE(CoordinatesMath::calculateDistance(closest, c1) < METERS_PRECISION);
}

TEST(ClosestPointTest, ClosestPointTest_Way_Test2) {
    Coordinates c1(Longitude(-2), Latitude(2));
    Coordinates c2(Longitude(-2), Latitude(1));
    Coordinates c3(Longitude(-3), Latitude(0.5));
    Coordinates c4(Longitude(0), Latitude(1));
    Coordinates c5(Longitude(-3), Latitude(1.3));
    Coordinates c6(Longitude(-5), Latitude(1.8));

    Coordinates target(Longitude(-6), Latitude(2));
    std::vector<Coordinates> vector {c1, c2, c3, c4, c5, c6};
    PointList points(vector);
    Coordinates closest = points.getClosestPoint(target);

    EXPECT_TRUE(CoordinatesMath::calculateDistance(closest, c6) < METERS_PRECISION);
}



TEST(CoordinatesMathTest, Shortest_Distance_And_Coordinates_Test) {
    Coordinates A(Longitude(-117.518921), Latitude(33.345678));
    Coordinates B(Longitude(-117.218492), Latitude(33.900678));
    Coordinates C(Longitude(-116.8), Latitude(38));
    Coordinates D = CoordinatesMath::closestCoordinatesOnSegment(A, B, C);
    Meters distance = CoordinatesMath::distanceFromSegment(A, B, C);
    std::pair<Meters, Coordinates> closestPair = CoordinatesMath::closestPointOnSegmentAndDistance(A, B, C);

    EXPECT_TRUE(abs(closestPair.first - distance) < METERS_PRECISION);
    EXPECT_TRUE(abs(CoordinatesMath::calculateDistance(C, D) - distance) < METERS_PRECISION);
    EXPECT_TRUE(CoordinatesMath::calculateDistance(closestPair.second, D) < METERS_PRECISION);

}

TEST(GISBasic, getWayClosestPoint_test1) {
    GIS gis;
    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\nyc.json");
    Coordinates coord(Longitude(40.731437), Latitude(-73.996967));
    Coordinates expected(Longitude(40.73248), Latitude(-73.99693));
    auto closest = gis.getWayClosestPoint(coord).first;
    EXPECT_TRUE(CoordinatesMath::calculateDistance(expected, closest) < METERS_PRECISION);
}

TEST(GISBasic, getWayClosestPoint_test2) {
    GIS gis;
    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\russia.json");
    Coordinates coord(Longitude(90.28674), Latitude(65.77863));
    Coordinates closest = gis.getWayClosestPoint(coord).first;
    Coordinates to(Longitude(90.28177), Latitude(65.89199));
    EXPECT_TRUE(CoordinatesMath::calculateDistance(to, closest) < METERS_PRECISION);
}

TEST(GISBasic, getWayClosestPoint_test3) {
    GIS gis;
    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\russia.json");
    Coordinates coord(Longitude(91.68265), Latitude(65.92547));
    Coordinates closest = gis.getWayClosestPoint(coord).first;
    Coordinates curve(Longitude(90.69101), Latitude(65.98046));
    EXPECT_TRUE(CoordinatesMath::calculateDistance(curve, closest) < METERS_PRECISION);
}
