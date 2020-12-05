#include <iostream>
#include "GIS.h"

using std::cout;
using std::endl;

int main() {
    GIS gis;
    gis.loadMapFile("/home/student/Desktop/advanced-ex1/russia.json");
    Coordinates coord(Longitude(90.28674), Latitude(65.77863));
    Coordinates to(Longitude(90.28177), Latitude(65.89199));

//    cout.precision(6);
//    for (auto &&cell: gis.grid->grid) {
//        cout << cell.first.longitude() << ", " << cell.first.latitude() << endl;
//    }
//
//
//    cout << "-------------------------------------------------------------------------------------------" << endl;
    Coordinates closest = gis.getWayClosestPoint(coord).first;
//
//    cout << closest.latitude() << ", " << to.latitude() << endl;
//    cout << closest.longitude() << ", " << to.longitude() << endl;

    return 0;
}
