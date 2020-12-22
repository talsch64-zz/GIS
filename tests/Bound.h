#ifndef GIS_CPP_BOUND_H
#define GIS_CPP_BOUND_H

#include "../GIS.h"

class Bound {
public:
    Latitude minLat;
    Latitude maxLat;
    Longitude minLon;
    Longitude maxLon;

    Bound(const Latitude &minLat, const Latitude &maxLat, const Longitude &minLon, const Longitude &maxLon);
};


#endif //GIS_CPP_BOUND_H
