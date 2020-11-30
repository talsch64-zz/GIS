#include <iostream>
#include "GIS.h"
#include "entities/JsonHandlers/Serializers/EntityJsonSerializer.h"
#include "entities/geometry/CoordinatesMath.h"
#include "entities/geometry/Circle.h"
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>



//#include <winbase.h>

//std::string getMapFilePath(const std::string &name);


int main(int argc, char* argv[]) {
//
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    Coordinates to(Longitude(90.28177), Latitude(65.89199));
    Coordinates coord(Longitude(91.68265), Latitude(65.92547));

    GIS gis;
    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\russia.json");
//    Coordinates closest = gis.getWayClosestPoint(to).first;
//    std::cout << closest.longitude() << ", " << closest.latitude() << std::endl;
    Coordinates closest = gis.getWayClosestPoint(coord).first;
    std::cout.precision(15);
    std::cout << closest.longitude() << ", " << closest.latitude() << std::endl;



//    [[40.733566, -73.999446]]
//    [40.732254, -73.996394]


//    auto way = gis.getWayClosestPoint(junction);
//
//    Grid grid;
//    Coordinates coord(Longitude(180), Latitude(10));
//    std::vector<Coordinates> neighbors = grid.getCellNeighbors(coord);
//    std::cout.precision(15);
//    for(int i = 0; i < neighbors.size(); ++i) {
//        std::cout << "longitude: " << neighbors[i].longitude() << " , latitude: " << neighbors[i].latitude() << std::endl;
//    }
//
//
//
//    std::string mapPath = "C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\4j1w.json";
//    std::vector<EntityId> entityId = gis.loadMapFile(mapPath);
//    gis.saveMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\output.json");
//
//
//    gis.saveMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\output.json");
//
//    std::vector<EntityId> ids = gis.getEntities("dsadsad");
//
//
//
//    Coordinates c1(Longitude(-3.0700117898814905), Latitude(58.643945458221));
//    Coordinates c2(Longitude(-5.714727651210689), Latitude(50.066393943909226));
//    Coordinates midPoint = CoordinatesMath::calculateMidpoint(c1, c2);
//
//    double distance = CoordinatesMath::calculateDistance(c1, midPoint);
//    double bearing1 = CoordinatesMath::calculateBearing(c1, midPoint);
//    double bearing2 = CoordinatesMath::calculateBearing(midPoint, c2);
//    Coordinates c2_cpy = CoordinatesMath::CoordinatesByBearingAndDistance(midPoint, Meters(distance), bearing2);
//
//    Coordinates c1(Longitude(-1), Latitude(10));
//    Coordinates c2(Longitude(-0.999), Latitude(10.001));
//    Coordinates c3(Longitude(-0.998), Latitude(10.001));
//
//
//    Coordinates c4(Longitude(-2), Latitude(1));
//    Coordinates c5(Longitude(-2.001), Latitude(1.001));
//    Coordinates c6(Longitude(-2), Latitude(1.0005));
//
//    std::vector<Coordinates> vector2 {c4, c5, c6};
//
//    PointList points2(vector2);
//    grid.setEntityOnGrid(points2, EntityId("dummy2"));
//
//
//    Coordinates A(Longitude(-117.518921), Latitude(35.345678));
//    Coordinates B(Longitude(-117.318492), Latitude(30.100678));
//    double bearing = CoordinatesMath::calculateBearing(A,B);
//    double b = CoordinatesMath::calculateBearing(B,A);
//    double distance = CoordinatesMath::calculateDistance(A,B);
//    Coordinates C = CoordinatesMath::CoordinatesByBearingAndDistance(B, Meters(distance), b);
//    double bearing2 = CoordinatesMath::calculateBearing(A,C);
//
//    Coordinates A(Longitude(-117.518921), Latitude(33.345678));
//    Coordinates B(Longitude(-117.218492), Latitude(33.900678));
//    Coordinates C(Longitude(-116.8), Latitude(38));
//    Coordinates D = CoordinatesMath::closestCoordinatesOnSegment(A, B, C);
//
//
//    Coordinates A(Longitude(-117.518921), Latitude(33.345678));
//    Coordinates B(Longitude(-117.218492), Latitude(33.900678));
//    Coordinates C(Longitude(-116.8), Latitude(38));
//    Coordinates D = CoordinatesMath::closestCoordinatesOnSegment(A, B, C);
//    Meters distance1 = CoordinatesMath::distanceFromSegment(A, B, C);
//    Meters distance2 = CoordinatesMath::calculateDistance(C, D);
//    std::pair<Meters, Coordinates> p = CoordinatesMath::calculateClosestPointAndDistanceFromLine(A, B, C);
//
//    Coordinates c = CoordinatesMath::closestCoordinatesOnSegment(c3, c2, c5);
//    Meters radius = CoordinatesMath::calculateDistance(c1, c2);
//    Circle circle(c2, radius);
//    c5  = circle.getClosestPoint(Coordinates {Longitude(4), Latitude(1)});
//
//
//    Coordinates c1(Longitude(-2), Latitude(2));
//    Coordinates c2(Longitude(-2), Latitude(1));
//    Coordinates c3(Longitude(-3), Latitude(0.5));
//    Coordinates c4(Longitude(0), Latitude(1));
//    Coordinates c5(Longitude(-3), Latitude(1.3));
//    Coordinates c6(Longitude(-5), Latitude(1.8));
////
//    Coordinates target(Longitude(-2.7), Latitude(1));
//    std::vector<Coordinates> vector {c1, c2, c3, c4, c5, c6};
//    PointList points(vector);
//    Coordinates closest = points.getClosestPoint(target);
//    Meters distance = CoordinatesMath::calculateDistance(closest, target);
//
//    Circle circle(c1, Meters(20000));
//    closest = circle.getClosestPoint(target);
//
//    Coordinates a1(Longitude(0.2545), Latitude(51.8853));
//    Coordinates b1(Longitude(2.5735), Latitude(49.0034));
//    Coordinates a2 = CoordinatesMath::CoordinatesByBearingAndDistance(a1, Meters(200000), 108.55);
//    Coordinates b2 = CoordinatesMath::CoordinatesByBearingAndDistance(b1, Meters(200000), 32.44);
//
//    Coordinates intersection = CoordinatesMath::intersection(a1, a2, b1, b2);
//
//
//
//    for (auto &&pair: gis.grid->grid) {
//        Coordinates cell = pair.first;
//        std::cout << "Longitude: " << cell.longitude() << ", Latitude: " << cell.latitude() << std::endl;
//    }


    return 0;
}
