

#ifndef EX1_ENTITY_H
#define EX1_ENTITY_H

#include <string>
#include <vector>
#include "GISdefs.h"

class Entity {
    EntityId id;
    std::string name;
    std::string description;
    std::vector<std::string> categoryTags;

public:
    Entity(const std::string &id, const std::string &name, const std::string &description,
           const std::vector<std::string> &category_tags);

    EntityId getId() { return id; }

    std::string getName() { return name; }

    std::string getDescription() { return description; }

    std::vector<std::string> getCategoryTags() { return categoryTags; }
};

#endif //EX1_ENTITY_H
