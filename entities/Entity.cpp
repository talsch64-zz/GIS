#include "Entity.h"
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

Entity::Entity(const std::string &id, const std::string &name, const std::string &description,
               const std::vector<std::string> &categoryTags, std::unique_ptr<Geometry> geometry, std::string type) : id(
        id), name(name), description(description),
                                                                                                                     categoryTags(
                                                                                                                             categoryTags),
                                                                                                                     geometry(
                                                                                                                             std::move(
                                                                                                                                     geometry)),
                                                                                                                     type(type),
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
