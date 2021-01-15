#pragma once

#include "../Navigation/Route.h"
#include "../GIS/GIS_315524694.h"

/**
 * @brief NavigationValidator class to test validity or Route
 *
 */
class NavigationValidator {
    const AbstractGIS &gis;


public:
    /**
     * @brief Construct a new Navigation Validator object
     *
     * @param gis - the referenced GIS object - Note that you get a const ref
     */
    NavigationValidator(const AbstractGIS &gis);

    /**
     * @brief Validates the given Route from Start Point to End Point on referenced GIS
     *
     * @param start - Start Point
     * @param end - End Point
     * @param routes - Route to be verified
     * @return true - Route is valid
     * @return false - Route is not valid
     */
    bool validateRoute(const Coordinates &start, const Coordinates &end, const AbstractRoute &routes) const;

    /**
     * @brief Validates the given Route with Restricitons from Start Point to End Point on referenced GIS
     *        Note: this is a bonus method - not mandatory unless you go for the avoid_toll bonus
     *
     * @param start - Start Point
     * @param end - End Point
     * @param restrictions - Restricions for Route
     * @param r - Route to be verified
     * @return true - Route is valid
     * @return false - Route is not valid
     */
    bool validateRoute(const Coordinates &start, const Coordinates &end, const Restrictions &restrictions,
                       const AbstractRoute &r) const;

private:
    /**
     * @brief returns a non-restricted way that is not an highway (unless the distance from coord is less then 3 meters)
     * @param coord
     * @param restrictions
     * @return tuple contains coordinates on the way, closest way's id itself and the initial segment number
     */
    std::tuple<Coordinates, EntityId, size_t>
    getClosestValidWay(const Coordinates &coord, const Restrictions &restrictions) const;

};

