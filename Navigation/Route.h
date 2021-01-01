#pragma once 

#include "../Common/GISNamedTypes.h"
#include "../Common/AbstractRoute.h"
#include <vector>

/**
 * @brief Route class
 * 
 */
class Route : public AbstractRoute {
public:
    ////////////////////////////////////////////////////////////////////////////////////
    //                                                                                //
    // TODO: For demo only - you may modify or remove the following empty constructor //
    //                                                                                //
    ////////////////////////////////////////////////////////////////////////////////////
    Route() {}

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

