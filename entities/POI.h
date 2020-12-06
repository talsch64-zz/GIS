
#ifndef EX1_POI_H
#define EX1_POI_H

#include <vector>
#include <string>
#include <memory>
#include "geometry/Geometry.h"
#include "Entity.h"

/// Entity derived class which represents a POI in the GIS application
/// POI's geometry is a Circle that is represented by a radius and center coordinates

class POI : public Entity {
    std::vector<std::string> accessibility;
public:
    POI(const EntityId &id, const std::string &name, const std::string &description,
        const std::vector<std::string> &categoryTags, std::vector<std::string> accessibility,
        std::unique_ptr<Geometry> geometry);

    const std::vector<std::string> &getAccessibility() const;

    rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) override;
};

#endif //EX1_POI_H
