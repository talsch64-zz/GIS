#include "Navigation.h"
#include <memory>
#include <iostream>

Navigation_098765432::Navigation_098765432(const NavigationGIS& gis) { (void)gis; }

/**
 * @brief Get the Routes object
 * 
 * @param start 
 * @param end 
 * @return Routes 
 */
std::unique_ptr<AbstractRoutes> Navigation_098765432::getRoutes(const Coordinates& start, const Coordinates& end) const {
    // A stub
    std::cout << "Navigation_098765432 is Getting routes from...\n";
    (void)start;
    (void)end;
    return std::make_unique<Routes>();
}


