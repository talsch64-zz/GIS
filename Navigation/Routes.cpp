#include "Routes.h" 

/**
 * @brief 
 * 
 * @return true - Routes are valid  
 * @return false - Routes not valid
 */
bool Routes::isValid() const { 
    // A stub
    return true; 
}

/**
 * @brief Retuns Route with shortest total distance
 *        Note: function can be called only if isValid is true - otherwise behavior is undefined
 * 
 * @return const Route& - shortest route in distance 
 */
const Route& Routes::shortestDistance() const {
    // A stub
    static Route shortest_route{};
    return shortest_route;
}

/**
 * @brief Retuns fastest Route
 *        Note: function can be called only if isValid is true - otherwise behavior is undefined
 * 
 * @return const Route& - shortest route in time 
 */
const Route& Routes::shortestTime() const {
    // A stub
    static Route fastest_route{};
    return fastest_route;
}
    
/**
 * @brief Get the Error Message 
 *        Note: function can be called only if isValid is false - otherwise behavior is undefined
 * 
 * @return const std::string& - error message
 */
const std::string& Routes::getErrorMessage() const {
    // A stub
    static std::string err{"oy vey!"};
    return err;
}

