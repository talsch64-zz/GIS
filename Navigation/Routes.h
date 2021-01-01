#pragma once 

#include "../Common/AbstractRoutes.h" 
#include "Route.h" 
#include <string> 

/**
 * @brief Routes class 
 * 
 */
class Routes : public AbstractRoutes {
public:
    ////////////////////////////////////////////////////////////////////////////////////
    //                                                                                //
    // TODO: For demo only - you may modify or remove the following empty constructor //
    //                                                                                //
    ////////////////////////////////////////////////////////////////////////////////////
    Routes() {}

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
    const Route& shortestDistance() const override;

    /**
     * @brief Retuns fastest Route
     *        Note: function can be called only if isValid is true - otherwise behavior is undefined
     * 
     * @return const Route& - shortest route in time 
     */
    const Route& shortestTime() const override;
    
    /**
     * @brief Get the Error Message 
     *        Note: function can be called only if isValid is false - otherwise behavior is undefined
     * 
     * @return const std::string& - error message
     */
    const std::string& getErrorMessage() const override;
};

