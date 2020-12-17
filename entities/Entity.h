

#ifndef EX1_ENTITY_H
#define EX1_ENTITY_H

#include <string>
#include <vector>
#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include <memory>
#include <optional>
#include "../GISNamedTypes.h"
#include "geometry/Geometry.h"

class EntityJsonSerializer;


/// Abstract class which is the base class of all the entities in the GIS application
/// In addition to the basic data members, each derived class has a Geometry (see derived classes documentation)
class Entity {
    EntityId id;
    std::string name;
    std::optional<std::string> description;
    std::vector<std::string> categoryTags;
    std::string type;

protected:
    EntityJsonSerializer *entityJsonSerializer;

public:
    Entity(EntityId id, std::string name, const std::string &description,
           std::vector<std::string> category_tags, std::string type);


    const EntityId &getId() const;

    const std::string &getName() const;

    const std::optional<std::string> &getDescription() const;

    const std::vector<std::string> &getCategoryTags() const;

    virtual rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) = 0;

    virtual ~Entity();

    void setId(const EntityId &id);

    virtual const std::unique_ptr<Geometry> &getGeometry() const = 0;

    const std::string &getType() const;

};

#endif //EX1_ENTITY_H
