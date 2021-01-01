#pragma once

#include "../Common/GISNamedTypes.h"
#include "../Common/AbstractWay.h"

/**
 * @brief Way class 
 * 
 */
class Way : public AbstractWay {
public:
    ////////////////////////////////////////////////////////////////////////////////////
    //                                                                                //
    // TODO: For demo only - you may modify or remove the following empty constructor //
    //                                                                                //
    ////////////////////////////////////////////////////////////////////////////////////
    Way() {}

    /**
     * @brief Get the Junctions object
     * 
     * @return std::pair<EntityId, EntityId> - <from_junction, to_junction>
     */
    std::pair<EntityId, EntityId> getJunctions() const override; 

    /**
     * @brief Get the Length of Way
     * 
     * @return Meters 
     */
    Meters getLength() const override; // might be lazily calculated (with a mutable field)
    
    /**
     * @brief 
     * 
     * @return true - Way is Bidirectional
     * @return false - Way is Unidirectional
     */
    bool isBidirectional() const override;

    /**
     * @brief Get the Speed Limit set on Way 
     * 
     * @return int 
     */
    int getSpeedLimit() const override;

    /**
     * @brief 
     * 
     * @return true - Way is an Highway 
     * @return false - Way is not an Highway
     */
    bool isHighway() const override;

    /**
     * @brief 
     * 
     * @return true - Way is a Toll Road 
     * @return false - Way is not a Toll Road
     */
    bool isToll() const override;
    
    /**
     * @brief Get the Segment Parts On Way object
     * 
     * @param segment 
     * @param c 
     * @return std::pair<Meters, Meters> - {from-junction->point, point->to-junction}
     */
    std::pair<Meters, Meters> getSegmentPartsOnWay(std::size_t segment, const Coordinates& c) const override;

    /**
     * @brief Get the From Junction Coordinates object
     * 
     * @return const Coordinates& 
     */
    const Coordinates& getFromJunctionCoordinates() const override;
    
    /**
     * @brief Get the To Junction Coordinates object
     * 
     * @return const Coordinates& 
     */
    const Coordinates& getToJunctionCoordinates() const override;
};

