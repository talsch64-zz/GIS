

#ifndef GIS_CPP_NAVIGATION_H
#define GIS_CPP_NAVIGATION_H

#include "Routes.h"
#include "NavigationGIS.h"

class Navigation {
public:
    Navigation(const NavigationGIS& gis); // note that you get a const ref
    Routes getRoutes(const Coordinates& start, const Coordinates& end) const;
    // bonus methods - not mandatory:
//    TODO implement method for bonus after Rectriction implementation
//    Routes getRoutes(
//            const Coordinates& start, const Coordinates& end, const Restrictions& res) const;
};



#endif //GIS_CPP_NAVIGATION_H
