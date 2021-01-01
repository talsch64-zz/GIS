#pragma once

#include "../Common/AbstractGIS.h"
#include "../Common/AbstractRoute.h"
#include "../Common/Restrictions2.h"

/**
 * @brief NavigationValidator class to test validity or Route
 * 
 */
class NavigationValidator {
public:
    /**
     * @brief Construct a new Navigation Validator object
     * 
     * @param gis - the referenced GIS object - Note that you get a const ref
     */
    NavigationValidator(const AbstractGIS& gis); 
    
    /**
     * @brief Validates the given Route from Start Point to End Point on referenced GIS
     * 
     * @param start - Start Point 
     * @param end - End Point
     * @param r - Route to be verified
     * @return true - Route is valid 
     * @return false - Route is not valid
     */
    bool validateRoute(const Coordinates& start, const Coordinates& end, const AbstractRoute& r) const;
};

