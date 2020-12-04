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
//    GIS gis;
//    auto loadedIds = gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\ways-search.json");
//    std::vector<EntityId> foundEntityIds = gis.getEntities("way",
//                                                           Coordinates(Longitude(-60.550612),
//                                                                       Latitude(-23.263132)),
//                                                           Meters(130000));
//    const Entity *way = gis.getEntityById(EntityId{"1"});
//    Coordinates c = Coordinates(Longitude(-60.550612),
//                                Latitude(-23.263132));
//    Coordinates closest = way->getGeometry()->getClosestPoint(c);
//    Meters distance = CoordinatesMath::calculateDistance(c, closest);
    GIS gis;
    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\json-files\\nyc.json");
    Coordinates coord(Longitude(40.731437), Latitude(-73.996967));
    Coordinates c1 {Longitude(40.732254),Latitude(-73.996394)};
    Coordinates c2 {Longitude(40.733566),Latitude(-73.999446)};
    Coordinates closest = CoordinatesMath::closestPointOnSegment(coord, c1, c2);
    std::cout.precision(10);
    std::cout << "closest: longitude: " << closest.longitude() << ", latitude: " << closest.latitude() << std::endl;
    double distance = CoordinatesMath::calculateDistance(closest, {Longitude(40.73225399999999), Latitude(-73.9963939999)});
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