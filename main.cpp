#include <iostream>
#include "GIS.h"
#include "entities/geometry/CoordinatesMath.h"

//#include <winbase.h>

//std::string getMapFilePath(const std::string &name);

using std::cout;
using std::endl;

int main() {
    Coordinates c1 = Coordinates(Longitude(40.732254), Latitude(-73.996394));
    Coordinates c2 = Coordinates(Longitude(40.733566), Latitude(-73.999446));
    Coordinates coord(Longitude(40.731437), Latitude(-73.996967));
//    Coordinates closest = CoordinatesMath::closestPointOnSegment(coord, c1, c2);
//    Coordinates closest2 = CoordinatesMath::closestPointOnSegment(coord, c2, c1);

//    cout << closest.longitude() << ", " << closest.latitude() << endl;
//    cout << closest2.longitude() << ", " << closest2.latitude() << endl;
    std::vector<Coordinates> v {c1, c2, c1};
    PointList points(v);
    Coordinates closest = points.getClosestPoint(coord);
    cout << closest.longitude() << ", " << closest.latitude() << endl;
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