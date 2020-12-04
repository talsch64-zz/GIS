#include <iostream>
#include "GIS.h"
#include "entities/geometry/CoordinatesMath.h"

//#include <winbase.h>

//std::string getMapFilePath(const std::string &name);


int main() {

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
    std::cout << closest.longitude() << ", " << closest.longitude() << std::endl;




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