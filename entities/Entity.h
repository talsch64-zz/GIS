

#ifndef EX1_ENTITY_H
#define EX1_ENTITY_H

#include <string>
#include <vector>
#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include <memory>
#include <optional>
#include "../GISdefs.h"
#include "geometry/Geometry.h"

class EntityJsonSerializer;

class Entity {
    EntityId id;
    std::string name;
    std::optional<std::string> description;
    std::vector<std::string> categoryTags;
    std::unique_ptr<Geometry> geometry;
    std::string type;

protected:
    EntityJsonSerializer *entityJsonSerializer;

public:
    Entity(const std::string &id, const std::string &name, const std::string &description,
           const std::vector<std::string> &category_tags, std::unique_ptr<Geometry> geometry, std::string type);

    const EntityId &getId() const;

    const std::string &getName() const;

    const std::optional<std::string> &getDescription() const;

    const std::vector<std::string> &getCategoryTags() const;

    virtual rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) = 0;

    virtual ~Entity();

    void setId(const EntityId &id);

    const std::unique_ptr<Geometry> &getGeometry() const;

    const std::string &getType() const;

};

#endif //EX1_ENTITY_H
