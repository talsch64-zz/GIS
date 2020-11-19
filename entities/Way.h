
#ifndef EX1_WAY_H
#define EX1_WAY_H

#include "Entity.h"
#include "../GISdefs.h"
#include "geometry/Geometry.h"

class Way: public Entity {
    EntityId from;
    EntityId to;
    std::string direction;
    int speedLimit;
    bool tollRoad;
    std::vector<std::string> restricted;

public:
    Way(const std::string &id, const std::string &name, const std::string &description,
        const std::vector<std::string> &categoryTags, std::unique_ptr<Geometry> geometry, const std::string &from,
        const std::string &to, const std::string &direction, int speedLimit, bool tollRoad, const std::vector<std::string> &restricted);

    const EntityId &getFrom() const;

    const EntityId &getTo() const;

    const std::string &getDirection() const;

    int getSpeedLimit() const;

    bool isTollRoad() const;

    const std::vector<std::string> &getRestricted() const;

    rapidjson::Value toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) override;
};

#endif //EX1_WAY_H
