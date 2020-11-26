#include <iostream>
#include "GIS.h"
#include "entities/JsonHandlers/Serializers/EntityJsonSerializer.h"
#include "entities/geometry/CoordinatesMath.h"
#include "entities/geometry/Circle.h"
#include <iostream>
#include <fstream>
#include <tchar.h>
//#include <winbase.h>

//std::string getMapFilePath(const std::string &name);


int main() {
    GIS gis;
//    std::string mapPath = "C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\noids.json";
//    std::vector<EntityId> entityId = gis.loadMapFile(mapPath);
//    gis.saveMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\output.json");
//
//    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\4j1w.json");
//
//    gis.saveMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\output.json");
//
//    std::vector<EntityId> ids = gis.getEntities("dsadsad");
//


//    Coordinates c1(Longitude(-3.0700117898814905), Latitude(58.643945458221));
//    Coordinates c2(Longitude(-5.714727651210689), Latitude(50.066393943909226));
//    Coordinates midPoint = CoordinatesMath::calculateMidpoint(c1, c2);

//    double distance = CoordinatesMath::calculateDistance(c1, midPoint);
//    double bearing1 = CoordinatesMath::calculateBearing(c1, midPoint);
//    double bearing2 = CoordinatesMath::calculateBearing(midPoint, c2);
//    Coordinates c2_cpy = CoordinatesMath::calculateCoordinatesByDistanceAndBearing(midPoint, Meters(distance), bearing2);

//    Coordinates c1(Longitude(-1), Latitude(10));
//    Coordinates c2(Longitude(-0.999), Latitude(10.001));
//    Coordinates c3(Longitude(-0.998), Latitude(10.001));
//
//    Grid grid;
//    std::vector<Coordinates> vector {c1, c2};
//    PointList points(vector);
//    grid.setEntityOnGrid(points, EntityId("dummy"));
//    grid.setEntityOnGrid(Point(Coordinates(Longitude(-1), Latitude(10))), EntityId("Junction"));
//
//    Coordinates c4(Longitude(-2), Latitude(1));
//    Coordinates c5(Longitude(-2.001), Latitude(1.001));
//    Coordinates c6(Longitude(-2), Latitude(1.0005));
//
//    std::vector<Coordinates> vector2 {c4, c5, c6};
//
//    PointList points2(vector2);
//    grid.setEntityOnGrid(points2, EntityId("dummy2"));


//    Coordinates A(Longitude(-117.518921), Latitude(35.345678));
//    Coordinates B(Longitude(-117.318492), Latitude(30.100678));
//    double bearing = CoordinatesMath::calculateBearing(A,B);
//    double b = CoordinatesMath::calculateBearing(B,A);
//    double distance = CoordinatesMath::calculateDistance(A,B);
//    Coordinates C = CoordinatesMath::calculateCoordinatesByDistanceAndBearing(B, Meters(distance), b);
//    double bearing2 = CoordinatesMath::calculateBearing(A,C);

//    Coordinates A(Longitude(-117.518921), Latitude(33.345678));
//    Coordinates B(Longitude(-117.218492), Latitude(33.900678));
//    Coordinates C(Longitude(-116.8), Latitude(38));
//    Coordinates D = CoordinatesMath::calculateClosestCoordinateAlongLine(A, B, C);


//    Coordinates A(Longitude(-117.518921), Latitude(33.345678));
//    Coordinates B(Longitude(-117.218492), Latitude(33.900678));
//    Coordinates C(Longitude(-116.8), Latitude(38));
//    Coordinates D = CoordinatesMath::calculateClosestCoordinateAlongLine(A, B, C);
//    Meters distance1 = CoordinatesMath::calculateShortestDistanceFromLine(A, B, C);
//    Meters distance2 = CoordinatesMath::calculateDistance(C, D);
//    std::pair<Meters, Coordinates> p = CoordinatesMath::calculateShortestDistanceAndCoordinatesFromLine(A, B, C);

    Coordinates c1(Longitude(-2), Latitude(2));
    Coordinates c2(Longitude(-2), Latitude(1));
    Coordinates c3(Longitude(-3), Latitude(0.5));
    Coordinates c4(Longitude(0), Latitude(1));
    Coordinates c5(Longitude(-3), Latitude(1.3));


    std::vector<Coordinates> vector2 {c1, c2, c3, c4};
    PointList points(vector2);
    Coordinates closest = points.getClosestPoint(c5);

    Coordinates c = CoordinatesMath::calculateClosestCoordinateAlongLine(c3, c2, c5);
    Meters radius = CoordinatesMath::calculateDistance(c1, c2);
    Circle circle(c2, radius);
    c5  = circle.getClosestPoint(Coordinates {Longitude(4), Latitude(1)});


    return 0;
}

//std::string getMapFilePath(const std::string &name) {
//    static TCHAR buffer[MAX_PATH];
//    static bool assigned = false;
//    if (!assigned) {
//        SetCurrentDirectory(_T(".."));
//        GetCurrentDirectory(MAX_PATH, buffer);
//        assigned = true;
//    }
//    std::string path = std::string(buffer) + "\\json-files\\" + name;
//    return path;
//}