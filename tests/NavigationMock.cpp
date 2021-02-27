#include "NavigationMock.h"

NavigationMock::NavigationMock(const NavigationGIS &navigationGis) : Navigation(navigationGis) {}

std::tuple<Coordinates, const AbstractWay &, std::size_t>
NavigationMock::getClosestValidWay(const Coordinates &coord, const Restrictions &restrictions) const {
    return Navigation::getClosestValidWay(coord, restrictions);
}
