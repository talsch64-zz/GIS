//
// Created by Tal on 05/11/2020.
//

#ifndef EX1_JUNCTION_H
#define EX1_JUNCTION_H

#include "Entity.h"
#include "../GISdefs.h"

class Junction: public Entity {
    Coordinates coordinates;
public:
    Junction(const std::string &id, const std::string &name, const std::string &description,
             const std::vector<std::string> &categoryTags, Coordinates coordinates);

    rapidjson::Value toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) override;

    const Coordinates &getCoordinates() const;
};

#endif //EX1_JUNCTION_H
