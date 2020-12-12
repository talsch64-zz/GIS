

#ifndef GIS_CPP_NAVIGATION_H
#define GIS_CPP_NAVIGATION_H

#include "Routes.h"
#include "NavigationGIS.h"

/// Navigation class manages the navagation between two Corrdinates
/// Supported operations:
/// * Get the shortest routes by distance and time
class Navigation {
    NavigationGIS gis;
public:


    Navigation(const NavigationGIS &gis);
    // note that you get a const ref
    /* returns Routes object which describes the shortest routes by time and distance */
//    TODO implement A*
    Routes getRoutes(const Coordinates& start, const Coordinates& end) const;

    // bonus methods - not mandatory:
//    TODO implement method for bonus after Rectriction implementation
//    Routes getRoutes(
//            const Coordinates& start, const Coordinates& end, const Restrictions& res) const;
};



#endif //GIS_CPP_NAVIGATION_H
