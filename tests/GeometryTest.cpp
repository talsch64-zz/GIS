#include "gtest/gtest.h"
#include "../GISNamedTypes.h"
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
    std::vector<Coordinates> vector{c1, c2, c3, c4, c5, c6};
    PointList points(vector);
    Coordinates closest = points.getClosestPoint(target);

    EXPECT_LT(CoordinatesMath::calculateDistance(closest, c1), METERS_PRECISION);
}

TEST(ClosestPointTest, ClosestPointTest_Way_Test2) {
    Coordinates c1(Longitude(-2), Latitude(2));
    Coordinates c2(Longitude(-2), Latitude(1));
    Coordinates c3(Longitude(-3), Latitude(0.5));
    Coordinates c4(Longitude(0), Latitude(1));
    Coordinates c5(Longitude(-3), Latitude(1.3));
    Coordinates c6(Longitude(-5), Latitude(1.8));

    Coordinates target(Longitude(-6), Latitude(2));
    std::vector<Coordinates> vector{c1, c2, c3, c4, c5, c6};
    PointList points(vector);
    Coordinates closest = points.getClosestPoint(target);

    EXPECT_LT(CoordinatesMath::calculateDistance(closest, c6), METERS_PRECISION);
}


TEST(CoordinatesMathTest, Shortest_Distance_And_Coordinates_Test) {
    Coordinates A(Longitude(-117.518921), Latitude(33.345678));
    Coordinates B(Longitude(-117.218492), Latitude(33.900678));
    Coordinates C(Longitude(-116.8), Latitude(38));
    Coordinates D = CoordinatesMath::closestPointOnSegment(C, A, B);
    Meters distance = CoordinatesMath::distanceFromSegment(C, A, B);
    std::pair<Coordinates, Meters> closestPair = CoordinatesMath::closestPointOnSegmentAndDistance(C, A, B);

    EXPECT_LT(abs(closestPair.second - distance), METERS_PRECISION);
    EXPECT_LT(abs(CoordinatesMath::calculateDistance(C, D) - distance), METERS_PRECISION);
    EXPECT_LT(CoordinatesMath::calculateDistance(closestPair.first, D), METERS_PRECISION);

}

TEST(CoordinatesTest, Longitude_Trim_Test) {
    Coordinates coords {Longitude(178), Latitude(0)};
    Coordinates target = CoordinatesMath::coordinatesByBearingAndDistance(coords, 90, Meters(200000));
    EXPECT_TRUE(target.longitude() > 0);
    double reverse_bearing = CoordinatesMath::initialBearing(target, coords);
    target = CoordinatesMath::coordinatesByBearingAndDistance(target, reverse_bearing, Meters(200000));
    EXPECT_EQ(coords.longitude(), target.longitude());
}
TEST(CoordinatesTest, ZulVern_Latitude_Test) {
    Coordinates coords {Longitude(0), Latitude(0)};
    Coordinates target = CoordinatesMath::coordinatesByBearingAndDistance(coords, 0, Meters(2*CoordinatesMath::pi*6371000));
    EXPECT_LT(abs(target.latitude() - coords.latitude()), 0.00001);
    EXPECT_EQ(target.longitude(), coords.longitude());
}

TEST(GISBasic, getWayClosestPoint_test1) {
    GIS gis;
    gis.loadMapFile("nyc.json");
    Coordinates coord(Longitude(40.731437), Latitude(-73.996967));
    Coordinates expected(Longitude(40.73248), Latitude(-73.99693));
    auto closest = gis.getWayClosestPoint(coord).first;
    EXPECT_LT(CoordinatesMath::calculateDistance(expected, closest), METERS_PRECISION);
}

TEST(GISBasic, getWayClosestPoint_test2) {
    GIS gis;
    gis.loadMapFile("russia.json");
    Coordinates coord(Longitude(90.28674), Latitude(65.77863));
    Coordinates closest = gis.getWayClosestPoint(coord).first;
    Coordinates to(Longitude(90.28177), Latitude(65.89199));
    EXPECT_TRUE(CoordinatesMath::calculateDistance(to, closest) < METERS_PRECISION);
}

TEST(GISBasic, getWayClosestPoint_test3) {
    GIS gis;
    gis.loadMapFile("russia.json");
    Coordinates coord(Longitude(91.68265), Latitude(65.92547));
    Coordinates closest = gis.getWayClosestPoint(coord).first;
    Coordinates curve(Longitude(90.69101), Latitude(65.98046));
    EXPECT_TRUE(CoordinatesMath::calculateDistance(curve, closest) < METERS_PRECISION);
}
