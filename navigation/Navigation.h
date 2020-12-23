
#pragma once

#include "Routes.h"
#include "../Restrictions.h"
#include "../NavigationGIS.h"

/**
 * @brief Navigation class.
 * Navigation class manages the navagation between two Corrdinates.
 * Supported operations:
 * Get the shortest routes by distance and time.
 *
 */
class Navigation {
    NavigationGIS navigationGIS;

public:
    /**
     * @brief Construct a new Navigation object
     *
     * @param navigationGis - Note: you get a const ref of NavigationGIS
     */
    Navigation(const NavigationGIS& navigationGis);

    /**
     * @brief Get the Routes object
     *
     * @param start
     * @param end
     * @return Routes
     */
    Routes getRoutes(const Coordinates& start, const Coordinates& end) const;

    /**
     * @brief Get the Routes object
     *        Note: bonus method - not mandatory
     *
     * @param start
     * @param end
     * @param restrictions - Restricted Ways
     * @return Routes
     */
    Routes getRoutes(const Coordinates& start, const Coordinates& end, const Restrictions& restrictions) const;
};
