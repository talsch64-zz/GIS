#include "gtest/gtest.h"
#include "../GISNamedTypes.h"
#include "../CoordinatesMath.h"

TEST(Double, SimplePrecisionTest) {
	Double<4> d1(1.12349487);
	Double<4> d2(1.12349487);
	EXPECT_EQ(d1, d2);
}

TEST(Double, ComparisonTest) {
    Double<5> d = 3.1234567;
	//supported:
    //std::cout << std::setprecision(10) << d << std::endl;
    //intentionally unsupported:
    //std::cout << (d == 3.12346) << std::endl;
    EXPECT_TRUE(d == Double<5>{3.12346});
    EXPECT_TRUE(!(d == Double<6>{3.1234567}));
    EXPECT_TRUE(d == Double<5>{3.1234567});
    EXPECT_TRUE(d == Double<6>{3.12346});
    EXPECT_TRUE(d == Double<6>{3.1234604});
    EXPECT_TRUE(d == Double<5>{3.1234567});
    EXPECT_TRUE(Double<6>{3.1234604} == d);
    EXPECT_TRUE(Double<6>{3.1234604} >= d);
    EXPECT_TRUE(Double<6>{3.1234604} <= d);
    EXPECT_TRUE(Double<6>{3.1234605} > d);
    EXPECT_TRUE(Double<6>{3.123459} < d);
    EXPECT_TRUE(!(Double<6>{3.1234604} > d));
}

TEST(Coordinates, ComparisonTest) {
	Coordinates coords = {Longitude{34}, Latitude{34}};
    Coordinates coords2 = {Longitude{34.0000004}, Latitude{34}};
    EXPECT_TRUE(coords == coords2);

    Coordinates coords3 = {Longitude{34.00000050000001}, Latitude{34}};
    EXPECT_NE(coords, coords3);

    auto [longitude, latitude] = coords;
    EXPECT_TRUE(longitude == Longitude{34.000000});
    EXPECT_TRUE(Latitude{34.000003} != latitude);
}

TEST(Coordinates, DistanceTest) {
	Coordinates c1(Longitude(1), Latitude(3));
	Coordinates c2(Longitude(-1), Latitude(4));

	EXPECT_EQ(CoordinatesMath::calculateDistance(c1, c2), Meters{248265.89});

	//First location (default: 1600 Pennsylvania Ave NW, Washington, DC)
	Coordinates coor1 = {Longitude(-77.037852), Latitude(38.898556)};
	Coordinates white_house = coor1;
	//Second location (default: 1922 F St NW, Washington, DC)
	Coordinates coor2 = {Longitude(-77.043934), Latitude(38.897147)};

	EXPECT_EQ(Meters{549.16}, CoordinatesMath::calculateDistance(coor1, coor2));
	EXPECT_EQ(Meters{0.0}, CoordinatesMath::calculateDistance(coor1, white_house));
}

TEST(Coordinates, DistanceTest2) {
	Coordinates athens = {Longitude{23.725750}, Latitude{37.971536}}; // Athens , Acropolis
	Coordinates brussels = {Longitude{4.3826169}, Latitude{50.8119483}}; // Brussels , ULB
	EXPECT_EQ(Meters{2085989.9}, CoordinatesMath::calculateDistance(athens, brussels));
}

TEST(Coordinates, MidpointVertical) {
    const double lng = -100;
	Coordinates almost_north_pole{Longitude{lng}, Latitude{89}};
	Coordinates almost_south_pole{Longitude{lng}, Latitude{-89}};
    auto midpoint = CoordinatesMath::midpoint(almost_north_pole, almost_south_pole);
    auto equator_point = Coordinates{Longitude{lng}, Latitude{0}};
	EXPECT_EQ(midpoint, equator_point);
}

TEST(Coordinates, MidpointHorizontal0) {
    const double lat = 0;
	Coordinates point1{Longitude{42}, Latitude{lat}};
	Coordinates point2{Longitude{84}, Latitude{lat}};
    auto midpoint = CoordinatesMath::midpoint(point1, point2);
    auto expected_midpoint = Coordinates{Longitude{63}, Latitude{lat}};
	EXPECT_EQ(midpoint, expected_midpoint);
}

TEST(Coordinates, MidpointHorizontal1) {
    const double lat = -10;
	Coordinates point1{Longitude{42}, Latitude{lat}};
	Coordinates point2{Longitude{84}, Latitude{lat}};
    auto midpoint = CoordinatesMath::midpoint(point1, point2);
    auto expected_midpoint = Coordinates{Longitude{63}, Latitude{-10.695563}};
	EXPECT_EQ(midpoint, expected_midpoint);
}

TEST(Coordinates, MidpointHorizontal2) {
    const double lat = 35;
	Coordinates point1{Longitude{135}, Latitude{lat}};
	Coordinates point2{Longitude{45}, Latitude{lat}};
    auto midpoint = CoordinatesMath::midpoint(point1, point2);
    auto expected_midpoint = Coordinates{Longitude{90}, Latitude{44.719114}};
	EXPECT_EQ(midpoint, expected_midpoint);
}

TEST(Coordinates, MidpointHorizontal3) {
    const double lat = 0;
	Coordinates point1{Longitude{20}, Latitude{lat}};
	Coordinates point2{Longitude{-160}, Latitude{lat}};
    auto midpoint = CoordinatesMath::midpoint(point1, point2);
    auto expected_midpoint = Coordinates{Longitude{-70}, Latitude{0}};
	EXPECT_EQ(midpoint, expected_midpoint);
}

TEST(Coordinates, MidpointDiagonal) {
	Coordinates point1{Longitude{120}, Latitude{45}};
	Coordinates point2{Longitude{40}, Latitude{-45}};
    auto midpoint = CoordinatesMath::midpoint(point1, point2);
    auto expected_midpoint = Coordinates{Longitude{80}, Latitude{0}};
	EXPECT_EQ(midpoint, expected_midpoint);
}

TEST(Coordinates, InitialBearingNorth) {
	Coordinates point1{Longitude{40}, Latitude{-65}};
	Coordinates point2{Longitude{40}, Latitude{5}};
    auto bearing = CoordinatesMath::initialBearing(point1, point2);
    auto expected_expected = 0;
	EXPECT_EQ(bearing, expected_expected);
}

TEST(Coordinates, InitialBearingSouth) {
	Coordinates point1{Longitude{40}, Latitude{45}};
	Coordinates point2{Longitude{40}, Latitude{-45}};
    auto bearing = CoordinatesMath::initialBearing(point1, point2);
    auto expected_expected = 180;
	EXPECT_EQ(bearing, expected_expected);
}

TEST(Coordinates, InitialBearingEast) {
    const double lat = 0;
	Coordinates point1{Longitude{40}, Latitude{lat}};
	Coordinates point2{Longitude{80}, Latitude{lat}};
    auto bearing = CoordinatesMath::initialBearing(point1, point2);
    auto expected_expected = 90;
	EXPECT_EQ(bearing, expected_expected);
}

TEST(Coordinates, InitialBearingWest) {
    const double lat = 0;
	Coordinates point1{Longitude{0}, Latitude{lat}};
	Coordinates point2{Longitude{-179}, Latitude{lat}};
    auto bearing = CoordinatesMath::initialBearing(point1, point2);
    auto expected_expected = 270;
	EXPECT_EQ(bearing, expected_expected);
}

TEST(Coordinates, InitialBearingEast2) {
    const double lat = 0;
	Coordinates point1{Longitude{2}, Latitude{lat}};
	Coordinates point2{Longitude{-179}, Latitude{lat}};
    auto bearing = CoordinatesMath::initialBearing(point1, point2);
    auto expected_expected = 90;
	EXPECT_EQ(bearing, expected_expected);
}

TEST(Coordinates, InitialBearingDiagonal) {
    const std::size_t precision = 10;
	Coordinates point1{Longitude{32.553212}, Latitude{23.445111}};
	Coordinates point2{Longitude{-10.342237}, Latitude{67.123999}};
    Double<precision> bearing = CoordinatesMath::initialBearing(point1, point2);
    Double<precision> expected_bearing = 340.125719401638;
	EXPECT_EQ(bearing, expected_bearing);
}

TEST(CoordinatesMath, closestPointOnSegmentFirst) {
    Coordinates point{Longitude{40}, Latitude{10}};
    Coordinates seg_1{Longitude{50}, Latitude{0}};
    Coordinates seg_2{Longitude{120}, Latitude{0}};
	Coordinates closest = CoordinatesMath::closestPointOnSegment(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{50}, Latitude{0}};
    EXPECT_EQ(closest, expected_closest);
}

TEST(CoordinatesMath, closestPointOnSegmentAlong) {
    Coordinates point{Longitude{40}, Latitude{10}};
    Coordinates seg_1{Longitude{80}, Latitude{0}};
    Coordinates seg_2{Longitude{-60}, Latitude{0}};
	auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{40}, Latitude{0}};
    EXPECT_EQ(closest.first, expected_closest);
    EXPECT_EQ(closest.first, CoordinatesMath::closestPointOnSegment(point, seg_2, seg_1));
    Meters dist1 = CoordinatesMath::distanceFromSegment(point, seg_2, seg_1);
    Meters dist2 = CoordinatesMath::calculateDistance(expected_closest, point);
    EXPECT_EQ(dist1, dist2);
}

TEST(CoordinatesMath, closestPointOnSegmentAlongMinus) {
    Coordinates point{Longitude{40}, Latitude{-10}};
    Coordinates seg_1{Longitude{80}, Latitude{0}};
    Coordinates seg_2{Longitude{-60}, Latitude{0}};
	auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{40}, Latitude{0}};
    EXPECT_EQ(closest.first, expected_closest);
    EXPECT_EQ(closest.first, CoordinatesMath::closestPointOnSegment(point, seg_2, seg_1));
    Meters dist1 = CoordinatesMath::distanceFromSegment(point, seg_2, seg_1);
    Meters dist2 = CoordinatesMath::calculateDistance(expected_closest, point);
    EXPECT_EQ(dist1, dist2);
}

TEST(CoordinatesMath, closestPointOnSegmentSecond) {
    Coordinates point{Longitude{40}, Latitude{10}};
    Coordinates seg_1{Longitude{-60}, Latitude{0}};
    Coordinates seg_2{Longitude{0}, Latitude{0}};
	Coordinates closest = CoordinatesMath::closestPointOnSegment(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{0}, Latitude{0}};
    EXPECT_EQ(closest, expected_closest);
}

TEST(CoordinatesMath, closestPointOnSegmentSameFirst) {
    Coordinates point{Longitude{40}, Latitude{10}};
    Coordinates seg_1{Longitude{40}, Latitude{10}};
    Coordinates seg_2{Longitude{39.999998}, Latitude{10.000001}};
    auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{40}, Latitude{10}};
    EXPECT_EQ(closest.first, expected_closest);
    EXPECT_EQ(closest.first, CoordinatesMath::closestPointOnSegment(point, seg_2, seg_1));
}

TEST(CoordinatesMath, closestPointOnSegmentSameFirst2) {
    Coordinates point{Longitude{-40}, Latitude{-10}};
    Coordinates seg_1{Longitude{-40}, Latitude{-10}};
    Coordinates seg_2{Longitude{-39.999998}, Latitude{-10.000001}};
	Coordinates closest = CoordinatesMath::closestPointOnSegment(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{-40}, Latitude{-10}};
    EXPECT_EQ(closest, expected_closest);
}

TEST(CoordinatesMath, closestPointOnSegmentSameSecond) {
    Coordinates point{Longitude{40}, Latitude{10}};
    Coordinates seg_1{Longitude{33}, Latitude{11}};
    Coordinates seg_2{Longitude{40}, Latitude{10}};
	Coordinates closest = CoordinatesMath::closestPointOnSegment(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{40}, Latitude{10}};
    EXPECT_EQ(closest, expected_closest);
}

TEST(CoordinatesMath, closestPointOnSegmentSameSecond2) {
    Coordinates point{Longitude{-40}, Latitude{-10}};
    Coordinates seg_1{Longitude{-33}, Latitude{-11}};
    Coordinates seg_2{Longitude{-40}, Latitude{-10}};
	Coordinates closest = CoordinatesMath::closestPointOnSegment(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{-40}, Latitude{-10}};
    EXPECT_EQ(closest, expected_closest);
}

TEST(CoordinatesMath, closestPointOnSegmentAndDistanceSecond) {
    Coordinates point{Longitude{40}, Latitude{10}};
    Coordinates seg_1{Longitude{-60}, Latitude{0}};
    Coordinates seg_2{Longitude{0}, Latitude{0}};
	auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{0}, Latitude{0}};
    EXPECT_EQ(closest.first, expected_closest);
    EXPECT_EQ(closest.second, CoordinatesMath::calculateDistance(point, seg_2));
    EXPECT_EQ(closest.second, CoordinatesMath::distanceFromSegment(point, seg_1, seg_2));
    EXPECT_EQ(closest.second, CoordinatesMath::distanceFromSegment(point, seg_2, seg_1));
}

TEST(CoordinatesMath, ClosestPointOnCircleInsideC) {
    Coordinates circle_center{Longitude{-20}, Latitude{30}};
    Meters circle_radius{2345.13};
    Coordinates c{Longitude{-20.001}, Latitude{30.002}};
	Coordinates closest = CoordinatesMath::closestPointOnCircle(c, circle_center, circle_radius);
    EXPECT_EQ(closest, c);
}

TEST(CoordinatesMath, ClosestPointOnCircleOutside) {
    Coordinates circle_center{Longitude{-20}, Latitude{30}};
    Meters circle_radius{22.23};
    Coordinates c{Longitude{-30.001}, Latitude{44.002}};
	Coordinates closest = CoordinatesMath::closestPointOnCircle(c, circle_center, circle_radius);
    auto dist_1 = CoordinatesMath::calculateDistance(circle_center, closest);
    auto dist_2 = CoordinatesMath::calculateDistance(c, closest);
    auto dist_3 = CoordinatesMath::calculateDistance(c, circle_center);
    EXPECT_EQ(CoordinatesMath::calculateDistance(circle_center, closest), Meters{22.18});
    EXPECT_TRUE(fabs(dist_1 + dist_2 - dist_3) < CoordinatesMath::distance_epsilon);
}

TEST(CoordinatesMath, ClosestPointOnCircleOutside2) {
    Coordinates circle_center{Longitude{20}, Latitude{0}};
    Meters circle_radius{22.01};
    Coordinates c{Longitude{-160}, Latitude{0}};
	Coordinates closest = CoordinatesMath::closestPointOnCircle(c, circle_center, circle_radius);
    auto dist_1 = CoordinatesMath::calculateDistance(circle_center, closest);
    auto dist_2 = CoordinatesMath::calculateDistance(c, closest);
    auto dist_3 = CoordinatesMath::calculateDistance(c, circle_center);
    EXPECT_EQ(CoordinatesMath::calculateDistance(circle_center, closest), Meters{22.02});
    EXPECT_EQ(dist_1 + dist_2, dist_3);
}

TEST(CoordinatesMath, Intersection) {
    Coordinates north{Longitude{30}, Latitude{80}};
    Coordinates south{Longitude{30}, Latitude{-80}};
    Coordinates west{Longitude{80}, Latitude{0}};
    Coordinates east{Longitude{-20}, Latitude{0}};
    auto intersection_point = CoordinatesMath::intersection(north, south, west, east);
    Coordinates expected_intersection_point{Longitude{30}, Latitude{0}};
    EXPECT_EQ(*intersection_point, expected_intersection_point);
}

TEST(CoordinatesMath, IntersectionWithMeridian) {
    Coordinates west{Longitude{80}, Latitude{0}};
    Coordinates east{Longitude{-20}, Latitude{0}};
    Latitude intersection_lat = CoordinatesMath::intersectionOnMeridian(west, east, Longitude{60});
    EXPECT_EQ(intersection_lat, Latitude{0});
    intersection_lat = CoordinatesMath::intersectionOnMeridian(east, west, Longitude{60});
    EXPECT_EQ(intersection_lat, Latitude{0});
}

TEST(CoordinatesMath, closestPointOnSegmentSameFirst3) {
    //From: https://stackoverflow.com/questions/32771458/distance-from-lat-lng-point-to-minor-arc-segment/35587935#35587935
    Coordinates p3{Longitude{-62.5}, Latitude{-10.5}};
    Coordinates p1{Longitude{-55.5}, Latitude{-10.1}};
    Coordinates p2{Longitude{-45.1}, Latitude{-15.2}};
    auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(p3, p1, p2);
    EXPECT_EQ(closest.first, p1);
    EXPECT_EQ(closest.second, Meters{767094.77});
}

TEST(CoordinatesMath, closestPointOnSegmentSecond2) {
    //From: https://stackoverflow.com/questions/32771458/distance-from-lat-lng-point-to-minor-arc-segment/35587935#35587935
    Coordinates p3{Longitude{42.0}, Latitude{25.0}};
    Coordinates p1{Longitude{35.61}, Latitude{21.72}};
    Coordinates p2{Longitude{40.70}, Latitude{23.65}};
    auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(p3, p1, p2);
    EXPECT_EQ(closest.first, p2);
    EXPECT_EQ(closest.second, Meters{199706.84});
}

TEST(CoordinatesMath, closestPointOnSegmentAlong2) {
    //From: https://stackoverflow.com/questions/32771458/distance-from-lat-lng-point-to-minor-arc-segment/35587935#35587935
    Coordinates p3{Longitude{69.0}, Latitude{51.0}};
    Coordinates p1{Longitude{60.5}, Latitude{40.5}};
    Coordinates p2{Longitude{80.5}, Latitude{50.5}};
    auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(p3, p1, p2);
    Coordinates expected_coordinates{Longitude{73.04979}, Latitude{47.475867}};
    EXPECT_EQ(closest.first, expected_coordinates);
    EXPECT_EQ(closest.second, Meters{479609.3});
}

