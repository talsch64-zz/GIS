#include "gtest/gtest.h"
#include "../GISNamedTypes.h"
#include "../CoordinatesMath.h"
#include "../entities/geometry/Circle.h"
#include "../entities/Way.h"
#include "../entities/geometry/PointList.h"
#include "../GIS.h"
#include "../NavigationGIS.h"
#include <numbers>

#define METERS_PRECISION Meters(1)
#define WAY_LENGTH_ACCEPTED_ERROR 0.005

TEST(ClosestPointTest, MyClosestPointWay1) {
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

TEST(ClosestPointTest, ClosestPointWay2) {
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


TEST(CoordinatesMathTest, ShortestDistanceAndCoordinatesTest) {
    Coordinates A(Longitude(-117.518921), Latitude(33.345678));
    Coordinates B(Longitude(-117.218492), Latitude(33.900678));
    Coordinates C(Longitude(-116.8), Latitude(38));
    Coordinates D = CoordinatesMath::closestPointOnSegment(C, A, B);
    Meters distance = CoordinatesMath::distanceFromSegment(C, A, B);
    std::pair<Coordinates, Meters> closestPair = CoordinatesMath::closestPointOnSegmentAndDistance(C, A, B);

    EXPECT_NEAR((double) closestPair.second, (double) distance, (double) METERS_PRECISION);
    EXPECT_NEAR((double) CoordinatesMath::calculateDistance(C, D), (double) distance, (double) METERS_PRECISION);
    EXPECT_LT(CoordinatesMath::calculateDistance(closestPair.first, D), METERS_PRECISION);
}

TEST(CoordinatesTest, LongitudeTrimTest) {
    Coordinates coords{Longitude(178), Latitude(0)};
    Coordinates target = CoordinatesMath::coordinatesByBearingAndDistance(coords, 90, Meters(200000));
    EXPECT_TRUE(target.longitude() > Longitude(0));
    double reverse_bearing = CoordinatesMath::initialBearing(target, coords);
    target = CoordinatesMath::coordinatesByBearingAndDistance(target, reverse_bearing, Meters(200000));
    EXPECT_EQ(coords.longitude(), target.longitude());
}

TEST(CoordinatesTest, ZulVernLatitudeTest) {
    Coordinates coords{Longitude(0), Latitude(0)};
    Coordinates target = CoordinatesMath::coordinatesByBearingAndDistance(coords, 0,
                                                                          Meters(2 * std::numbers::pi * 6371000));
    EXPECT_NEAR((double) target.latitude(), (double) coords.latitude(), 0.00001);
    EXPECT_EQ(target.longitude(), coords.longitude());
}

TEST(GISBasic, MygetWayClosestPointTest1) {
    GIS gis;
    gis.loadMapFile("nyc2.json");
    Coordinates coord(Longitude(40.731437), Latitude(-73.996967));
    Coordinates expected(Longitude(40.73248), Latitude(-73.99693));
    auto closest = gis.getWayClosestPoint(coord).first;
    EXPECT_LT(CoordinatesMath::calculateDistance(expected, closest), METERS_PRECISION);
}

TEST(GISBasic, MygetWayClosestPointTest2) {
    GIS gis;
    gis.loadMapFile("russia.json");
    Coordinates coord(Longitude(90.28674), Latitude(65.77863));
    Coordinates closest = gis.getWayClosestPoint(coord).first;
    Coordinates to(Longitude(90.28177), Latitude(65.89199));
    EXPECT_TRUE(CoordinatesMath::calculateDistance(to, closest) < METERS_PRECISION);
}

TEST(GISBasic, MygetWayClosestPointTest3) {
    GIS gis;
    gis.loadMapFile("russia.json");
    Coordinates coord(Longitude(91.68265), Latitude(65.92547));
    Coordinates closest = gis.getWayClosestPoint(coord).first;
    Coordinates curve(Longitude(90.69101), Latitude(65.98046));
    EXPECT_TRUE(CoordinatesMath::calculateDistance(curve, closest) < METERS_PRECISION);
}

TEST(WayGeometry, GetWayLengthWithCurves) {
    GIS gis;
    gis.loadMapFile("ways.json");
    Meters expectedLength(43633);
    Meters acceptedError = WAY_LENGTH_ACCEPTED_ERROR * expectedLength;

    Way *way = (Way *) gis.getEntityById(EntityId("1"));
    Meters length = way->getLength();

    EXPECT_NEAR((double) length, (double) expectedLength, (double) acceptedError);
}

TEST(WayGeometry, GetWayLengthWithoutCurves) {
    GIS gis;
    gis.loadMapFile("ways.json");
    Meters expectedLength(21223);
    Meters acceptedError = WAY_LENGTH_ACCEPTED_ERROR * expectedLength;

    Way *way = (Way *) gis.getEntityById(EntityId("2"));
    Meters length = way->getLength();

    EXPECT_NEAR((double) length, (double) expectedLength, (double) acceptedError);
}

TEST(ClosestWay, ClosestWayWithRestrictions) {
    GIS gis;
    NavigationGIS navigationGis(gis);
    Coordinates coord(Longitude(23.916374), Latitude(-30.671331));
    gis.loadMapFile("astar.json");

    Restrictions restrictions1("highway  , toll");
    Restrictions restrictions2("  highway ");
    Restrictions restrictions3("toll");
    Restrictions restrictions4("");
    auto closestWay1 = navigationGis.getWayClosestPoint(coord, restrictions1);
    auto closestWay2 = navigationGis.getWayClosestPoint(coord, restrictions2);
    auto closestWay3 = navigationGis.getWayClosestPoint(coord, restrictions3);
    auto closestWay4 = navigationGis.getWayClosestPoint(coord, restrictions4);

    ASSERT_EQ(closestWay1.second, EntityId("way1"));
    ASSERT_EQ(closestWay2.second, EntityId("way8"));
    ASSERT_EQ(closestWay3.second, EntityId("way1"));
    ASSERT_EQ(closestWay4.second, EntityId("way8"));
}
