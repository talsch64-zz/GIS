
#pragma once

#include <string>
#include <memory>
#include "Route.h"
#include "../Common/GISNamedTypes.h"
#include "../Common/AbstractRoutes.h"


/** Routes class hold the two shortest routes - by distance and time.
 * Operation supported:
 * get shortest route by distance and by time,
 * are the shortest routes valid,
 * get error message in case the routes are not valid.
 */
class Routes : public AbstractRoutes {
    std::unique_ptr<AbstractRoute> shortestDistanceRoute;
    std::unique_ptr<AbstractRoute> shortestTimeRoute;
public:
    Routes(std::unique_ptr<AbstractRoute> shortestDistanceRoute, std::unique_ptr<AbstractRoute> shortestTimeRoute,
           bool valid,
           const std::string &errorMessage);

    /**
     * @brief Constructor for Routes Object
     */
    Routes(std::unique_ptr<AbstractRoute> shortestDistanceRoute, std::unique_ptr<AbstractRoute> shortestTimeRoute,
           bool valid);


private:
    bool valid;
    std::string errorMessage;
public:
    /**
     * @brief Constructor for invalid Route;
     */
    Routes();


    /**
     * @brief
     *
     * @return true - Routes are valid
     * @return false - Routes not valid
     */
    bool isValid() const override;

    /**
     * @brief Retuns Route with shortest total distance
     *        Note: function can be called only if isValid is true - otherwise behavior is undefined
     *
     * @return const Route& - shortest route in distance
     */
    const AbstractRoute &shortestDistance() const override;

    /**
    * @brief Retuns fastest Route
    *        Note: function can be called only if isValid is true - otherwise behavior is undefined
    *
    * @return const Route& - shortest route in time
    */
    const AbstractRoute &shortestTime() const override;

    // following function can be called only if isValid is false - undefined otherwise
    /**
        * @brief Get the Error Message
        *        Note: function can be called only if isValid is false - otherwise behavior is undefined
        *
        * @return const std::string& - error message
        */
    const std::string &getErrorMessage() const override;
};


