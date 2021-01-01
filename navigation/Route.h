
#pragma once

#include <vector>
#include "../Common/GISNamedTypes.h"

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

 class Route {
 protected:
     Coordinates startPoint;
     Coordinates endPoint;
     Meters length;
     Minutes duration;
     std::vector<std::pair<EntityId, Direction>> ways;
     bool valid;

 public:
     /**
     * @brief Constructor for Route
     */
     Route(Coordinates startPoint, Coordinates endPoint, Meters length, Minutes duration, std::vector<std::pair<EntityId, Direction>> ways, bool valid);

     /**
     * @brief Get the ways constructing the Route and direction per way
     *
     * @return const std::vector<std::pair<EntityId, Direction>>&
     */
    const std::vector<std::pair<EntityId, Direction>>& getWays() const;

    /**
     * @brief Get the Way's Start Point
     *
     * @return const Coordinates&
     */
    const Coordinates& getWayStartPoint() const;

    /**
     * @brief Get the Way's End Point
     *
     * @return const Coordinates&
     */
    const Coordinates& getWayEndPoint() const;

    /**
     * @brief Get Way's total length
     *
     * @return Meters
     */
    Meters totalLength() const;

    /**
     * @brief Get Way's estimated duration
     *
     * @return Minutes
     */
    Minutes estimatedDuration() const;

     /**
      * @brief Check if route is valid
      *
      * @return true if route is valid, else false
      */
     bool isValid() const;


     /**
      * @brief initializing invalid Route
      * @param start
      * @param end
      * @return invalid route
      */
     static Route invalidRoute(const Coordinates &start, const Coordinates &end);
     /**
      * @brief initializing invalid Route
      * @return invalid route
      */
     static Route invalidRoute();


 };



