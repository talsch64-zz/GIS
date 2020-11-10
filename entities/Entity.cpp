#include "Entity.h"

Entity::Entity(const std::string &id, const std::string &name, const std::string &description,
               const std::vector<std::string> &categoryTags) : id(id), name(name), description(description),
                                                               categoryTags(categoryTags) {}
