//
// Created by Tal on 05/11/2020.
//

#ifndef EX1_JUNCTION_H
#define EX1_JUNCTION_H

#include "Entity.h"
#include "../Common/GISNamedTypes.h"
#include "geometry/Point.h"


/// Entity derived class which represents a junction in the GIS application
/// Junction's Geometry is a Point which holds a single Coordinates
class Junction: public Entity {
    std::unique_ptr<Point> geometry;
    /* vector with all the ways that cross the junction */
    std::vector<EntityId> ways;

public:
    Junction(const EntityId &id, const std::string &name, const std::string &description,
             const std::vector<std::string> &categoryTags, std::unique_ptr<Point> geometry);

    rapidjson::Value toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) override;

    const std::unique_ptr<Geometry> &getGeometry() const override;

    /* adds ways id to ways */
    void addWay(const EntityId &wayId);

    const std::vector<EntityId> &getWays() const;

};

#endif //EX1_JUNCTION_H
