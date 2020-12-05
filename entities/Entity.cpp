#include "Entity.h"

#include <utility>
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

Entity::Entity(EntityId id, std::string name, const std::string &description,
               std::vector<std::string> categoryTags, std::unique_ptr<Geometry> geometry, std::string type) : id(std::move(
        id)), name(std::move(name)), description(description),
                                                                                                                     categoryTags(std::move(
                                                                                                                             categoryTags)),
                                                                                                                     geometry(
                                                                                                                             std::move(
                                                                                                                                     geometry)),
                                                                                                                     type(std::move(type)),
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

const std::unique_ptr<Geometry> &Entity::getGeometry() const {
    return geometry;
}

const std::string &Entity::getType() const {
    return type;
}
