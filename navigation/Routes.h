
#ifndef GIS_CPP_ROUTES_H
#define GIS_CPP_ROUTES_H

#include "Route.h"
#include "../GISNamedTypes.h"


/// Routes class hold the two shortest routes - by distance and time
/// Operation supported:
/// * get shortest route by distance and by time
/// * are the shortest routes valid
/// * get error message in case the routes are not valid
class Routes {
    Route shortestDistanceRoute;
    Route shortestTimeRoute;
    bool valid;

public:
    bool isValid() const; // is route valid

    // following functions can be called only if isValid is true - undefined otherwise
    const Route& shortestDistance() const;
    const Route& shortestTime() const;

    // following function can be called only if isValid is false - undefined otherwise
//    TODO implement
    const std::string& getErrorMessage() const;
};


#endif //GIS_CPP_ROUTES_H
