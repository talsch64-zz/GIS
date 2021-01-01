#include "NavigationValidator.h"

/**
 * @brief Construct a new Navigation Validator object
 * 
 * @param gis - the referenced GIS object - Note that you get a const ref
 */
NavigationValidator::NavigationValidator(const AbstractGIS& gis) { (void)gis; } 

/**
 * @brief Validates the given Route from Start Point to End Point on referenced GIS
 * 
 * @param start - Start Point 
 * @param end - End Point
 * @param r - Route to be verified
 * @return true - Route is valid 
 * @return false - Route is not valid
 */
bool NavigationValidator::validateRoute(const Coordinates& start, const Coordinates& end, const AbstractRoute& r) const {
    (void)start;
    (void)end; 
    (void)r;
    return true;
}


