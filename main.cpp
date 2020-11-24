#include <iostream>
#include "GIS.h"
#include "entities/JsonHandlers/Serializers/EntityJsonSerializer.h"
#include "entities/geometry/CoordinatesMath.h"
#include <iostream>
#include <fstream>
#include <tchar.h>
//#include <winbase.h>

//std::string getMapFilePath(const std::string &name);


int main() {
//    GIS gis;
//    std::string mapPath = "C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\noids.json";
//    std::vector<EntityId> entityId = gis.loadMapFile(mapPath);
//    gis.saveMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\output.json");

//    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\3j1w1p.json");

//    Coordinates c1(Longitude(-3.0700117898814905), Latitude(58.643945458221));
//    Coordinates c2(Longitude(-5.714727651210689), Latitude(50.066393943909226));
//    Coordinates midPoint = CoordinatesMath::calculateMidpoint(c1, c2);
////    Coordinates c1(Longitude(-1), Latitude(10));
////    Coordinates c2(Longitude(-0.8708031656252585), Latitude(10.129101382398709));
//    Coordinates tal(Longitude(34.87630298242422), Latitude(31.938347454762326));
//    Coordinates sonia(Longitude(35.00539233396715), Latitude(31.89113667156286));
//    double d = CoordinatesMath::calculateDistance(tal, sonia);
//
//    double distance = CoordinatesMath::calculateDistance(c1, midPoint);
//    double bearing1 = CoordinatesMath::calculateBearing(c1, midPoint);
//    double bearing2 = CoordinatesMath::calculateBearing(midPoint, c2);
//    Coordinates c2_cpy = CoordinatesMath::calculateCoordinateByDistance(midPoint, Meters(distance), bearing2);

//    Coordinates c1(Longitude(-1), Latitude(10));
//    Coordinates c2(Longitude(-0.999), Latitude(10.001));
//    Coordinates c3(Longitude(-0.998), Latitude(10.001));

//
//    Grid grid;
//    std::vector<Coordinates> vector {c1, c3, c2};
//    PointList points(vector);
//    grid.setEntityOnGrid(points, EntityId("dummy"));

//    Coordinates A(Longitude(-117.518921), Latitude(35.345678));
//    Coordinates B(Longitude(-117.318492), Latitude(30.100678));
//    double bearing = CoordinatesMath::calculateBearing(A,B);
//    double b = CoordinatesMath::calculateBearing(B,A);
//    double distance = CoordinatesMath::calculateDistance(A,B);
//    Coordinates C = CoordinatesMath::calculateCoordinateByDistance(B, Meters(distance), b);
//    double bearing2 = CoordinatesMath::calculateBearing(A,C);

    Coordinates A(Longitude(-117.518921), Latitude(33.345678));
    Coordinates B(Longitude(-117.218492), Latitude(33.900678));
    Coordinates C(Longitude(-116.8), Latitude(38));
    Coordinates D = CoordinatesMath::calculateClosestCoordinateAlongLine(A, B, C);

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