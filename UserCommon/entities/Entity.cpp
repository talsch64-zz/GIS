#include "Entity.h"

#include <utility>
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

Entity::Entity(EntityId id, std::string name, const std::string &description,
               std::vector<std::string> categoryTags, std::string type) : id(std::move(
        id)), name(std::move(name)), description(description), categoryTags(std::move(categoryTags)),
                                                                          type(std::move(type)), entityJsonSerializer(
                std::make_unique<EntityJsonSerializer>()) {}

Entity::~Entity() {
}

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

const std::string &Entity::getType() const {
    return type;
}
