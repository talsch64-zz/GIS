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

//    std::string mapPath = "C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\4j1w.json";
//    std::vector<EntityId> entityId = gis.loadMapFile(mapPath);
//    gis.saveMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\output.json");
//
//
//    gis.saveMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\output.json");
//
//
//    for (auto &&pair: gis.grid->grid) {
//        Coordinates cell = pair.first;
//        std::cout << "Longitude: " << cell.longitude() << ", Latitude: " << cell.latitude() << std::endl;
//    }


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