#ifndef GIS_CPP_NAVIGATIONMOCK_H
#define GIS_CPP_NAVIGATIONMOCK_H


#include "../Navigation/Navigation.h"

class NavigationMock: public Navigation {
public:
    NavigationMock(const NavigationGIS &navigationGis);

    std::tuple<Coordinates, const AbstractWay &, std::size_t>
    getClosestValidWay(const Coordinates &coord, const Restrictions &restrictions) const;
};


#endif //GIS_CPP_NAVIGATIONMOCK_H
