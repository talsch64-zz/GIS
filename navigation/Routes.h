
#pragma once

#include <string>
#include "Route.h"
#include "../GISNamedTypes.h"


    /** Routes class hold the two shortest routes - by distance and time.
     * Operation supported:
     * get shortest route by distance and by time,
     * are the shortest routes valid,
     * get error message in case the routes are not valid.
     */
class Routes {
    Route shortestDistanceRoute;
    Route shortestTimeRoute;
    bool valid;
public:
    /**
     * @brief Constructor for Routes Object
     *
     */
    Routes(const Route &shortestDistanceRoute, const Route &shortestTimeRoute, bool valid);



    /**
     * @brief
     *
     * @return true - Routes are valid
     * @return false - Routes not valid
     */
    bool isValid() const;

    /**
     * @brief Retuns Route with shortest total distance
     *        Note: function can be called only if isValid is true - otherwise behavior is undefined
     *
     * @return const Route& - shortest route in distance
     */
    const Route& shortestDistance() const;
    /**
    * @brief Retuns fastest Route
    *        Note: function can be called only if isValid is true - otherwise behavior is undefined
    *
    * @return const Route& - shortest route in time
    */
    const Route& shortestTime() const;

    // following function can be called only if isValid is false - undefined otherwise
    /**
        * @brief Get the Error Message
        *        Note: function can be called only if isValid is false - otherwise behavior is undefined
        *
        * @return const std::string& - error message
        */
    const std::string& getErrorMessage() const;
};


