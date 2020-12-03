#include <iostream>
#include "GIS.h"
#include "entities/JsonHandlers/Serializers/EntityJsonSerializer.h"
#include "entities/geometry/CoordinatesMath.h"
#include "entities/geometry/Circle.h"
#include <iostream>
#include <fstream>
#include <tchar.h>
//#include "tests/lib/googletest/gte"




//#include <winbase.h>

//std::string getMapFilePath(const std::string &name);


int main(int argc, char* argv[]) {
    GIS gis;
    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\nyc.json");
    Coordinates coord(Longitude(40.731437), Latitude(-73.996967));
    Coordinates expected(Longitude(40.73248), Latitude(-73.99693));
    auto closest = gis.getWayClosestPoint(coord).first;

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