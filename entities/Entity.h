

#ifndef EX1_ENTITY_H
#define EX1_ENTITY_H

#include <string>
#include <vector>
#include "../GISdefs.h"

class Entity {
    EntityId id;
    std::string name;
    std::string description;
    std::vector<std::string> categoryTags;

public:
    Entity(const std::string &id, const std::string &name, const std::string &description,
           const std::vector<std::string> &category_tags);
    virtual ~Entity() = 0;

    EntityId getId() { return id; }

};

#endif //EX1_ENTITY_H
