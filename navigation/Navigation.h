
#pragma once

#include "NavigationGIS.h"
#include "Routes.h"
#include "../Restrictions.h"

/**
 * @brief Navigation class.
 * Navigation class manages the navagation between two Corrdinates.
 * Supported operations:
 * Get the shortest routes by distance and time.
 *
 */
class Navigation {
    NavigationGIS gis;

public:
    /**
     * @brief Construct a new Navigation object
     *
     * @param gis - Note: you get a const ref of NavigationGIS
     */
    Navigation(const NavigationGIS& gis);

    /**
     * @brief Get the Routes object
     *
     * @param start
     * @param end
     * @return Routes
     */
    //    TODO implement A*
    Routes getRoutes(const Coordinates& start, const Coordinates& end) const;

    /**
     * @brief Get the Routes object
     *        Note: bonus method - not mandatory
     *
     * @param start
     * @param end
     * @param res - Restricted Ways
     * @return Routes
     */
    //    TODO implement method for bonus after Rectriction implementation
    Routes getRoutes(const Coordinates& start, const Coordinates& end, const Restrictions& res) const;
};
