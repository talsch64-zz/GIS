#include "NavigationMock.h"

NavigationMock::NavigationMock(const NavigationGIS &navigationGis) : Navigation_315524694(navigationGis) {}

std::tuple<Coordinates, const AbstractWay &, std::size_t>
NavigationMock::getClosestValidWay(const Coordinates &coord, const Restrictions &restrictions) const {
    return Navigation_315524694::getClosestValidWay(coord, restrictions);
}
