//
// Created by Tal on 05/11/2020.
//

#ifndef EX1_JUNCTION_H
#define EX1_JUNCTION_H

#include "Entity.h"
#include "../GISdefs.h"
#include "geometry/Point.h"

class Junction: public Entity {
public:
    Junction(const EntityId &id, const std::string &name, const std::string &description,
             const std::vector<std::string> &categoryTags, std::unique_ptr<Geometry> geometry);

    rapidjson::Value toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) override;
};

#endif //EX1_JUNCTION_H
