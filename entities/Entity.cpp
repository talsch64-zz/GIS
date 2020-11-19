#include "Entity.h"
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

Entity::Entity(const std::string &id, const std::string &name, const std::string &description,
               const std::vector<std::string> &categoryTags) : id(id), name(name), description(description),
                                                               categoryTags(categoryTags),
                                                               entityJsonSerializer(
                                                                       new EntityJsonSerializer()) {}

const EntityId &Entity::getId() const {
    return id;
}

const std::string &Entity::getName() const {
    return name;
}

const std::optional<std::string> &Entity::getDescription() const {
    return description;
}

const std::vector<std::string> &Entity::getCategoryTags() const {
    return categoryTags;
}

Entity::~Entity() {
    delete entityJsonSerializer;
}

void Entity::setId(const EntityId &id) {
    Entity::id = id;
}