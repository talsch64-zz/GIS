#include <iostream>
#include "GIS.h"
#include "entities/geometry/CoordinatesMath.h"

//#include <winbase.h>

//std::string getMapFilePath(const std::string &name);

using std::cout;
using std::endl;

int main() {

    Coordinates c1(Longitude(-2), Latitude(2));
    Coordinates c2(Longitude(-2), Latitude(1));
    Coordinates c3(Longitude(-3), Latitude(0.5));
    Coordinates c4(Longitude(0), Latitude(1));
    Coordinates c5(Longitude(-3), Latitude(1.3));
    Coordinates c6(Longitude(-5), Latitude(1.8));

    cout << c1.longitude() << ", " << c1.latitude() << endl;
    cout << c2.longitude() << ", " << c2.latitude() << endl;
    cout << c3.longitude() << ", " << c3.latitude() << endl;
    cout << c4.longitude() << ", " << c4.latitude() << endl;
    cout << c5.longitude() << ", " << c5.latitude() << endl;
    cout << c6.longitude() << ", " << c6.latitude() << endl;



    Coordinates target(Longitude(-1), Latitude(4));
    std::vector<Coordinates> vector{c1, c2, c3, c4, c5, c6};
    PointList points(vector);
    Coordinates closest = points.getClosestPoint(target);
    std::cout << closest.longitude() << ", " << closest.latitude() << std::endl;

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