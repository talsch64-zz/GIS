#include "Route.h"

/**
 * @brief Get the ways constructing the Route and direction per way
 * 
 * @return const std::vector<std::pair<EntityId, Direction>>&
 */
const std::vector<std::pair<EntityId, Direction>>& Route::getWays() const {
    static std::vector<std::pair<EntityId, Direction>> ways{std::make_pair(EntityId{"W1"}, Direction::A_to_B), std::make_pair(EntityId{"W3"}, Direction::A_to_B), std::make_pair(EntityId{"W5"}, Direction::A_to_B)};
    return ways;
}

/**
 * @brief Get the Way's Start Point 
 * 
 * @return const Coordinates& 
 */
const Coordinates& Route::getWayStartPoint() const {
    static Coordinates res{Longitude{-30.23}, Latitude{49.84}};
    return res;
}
    
/**
 * @brief Get the Way's End Point
 * 
 * @return const Coordinates& 
 */
const Coordinates& Route::getWayEndPoint() const {
    static Coordinates res{Longitude{-25.4215375487951}, Latitude{48.5125210287639}};
    return res;
}
    
/**
 * @brief Get Way's total length
 * 
 * @return Meters 
 */
Meters Route::totalLength() const {
    return Meters{222.0};
}

/**
 * @brief Get Way's estimated duration
 * 
 * @return Minutes 
 */
Minutes Route::estimatedDuration() const {
    return Minutes{222.0};
}


