
#pragma once

#include <vector>
#include <memory>
#include "../Common/GISNamedTypes.h"
#include "../Common/AbstractRoute.h"

/**
 * @brief Route class
 * Route class describes a route between two points and the ways in which the route goes through.
 * Information available about the Route:
 * Distance (in Meters),
 * Duration (in Minutes),
 * Start and end points,
 * All the ways in which the route goes through (and the directions),
 * Is the route valid.
 */

 class Route: public AbstractRoute {
 protected:
     Coordinates startPoint;
     Coordinates endPoint;
     Meters length;
     Minutes duration;
     std::vector<std::pair<EntityId, Direction>> ways;

 public:
     /**
     * @brief Constructor for Route
     */
     Route(Coordinates startPoint, Coordinates endPoint, Meters length, Minutes duration, std::vector<std::pair<EntityId, Direction>> ways);

     /**
     * @brief Get the ways constructing the Route and direction per way
     *
     * @return const std::vector<std::pair<EntityId, Direction>>&
     */
    const std::vector<std::pair<EntityId, Direction>>& getWays() const override;

    /**
     * @brief Get the Way's Start Point
     *
     * @return const Coordinates&
     */
    const Coordinates& getWayStartPoint() const override;

    /**
     * @brief Get the Way's End Point
     *
     * @return const Coordinates&
     */
    const Coordinates& getWayEndPoint() const override;

    /**
     * @brief Get Way's total length
     *
     * @return Meters
     */
    Meters totalLength() const override;

    /**
     * @brief Get Way's estimated duration
     *
     * @return Minutes
     */
    Minutes estimatedDuration() const override;
 };



