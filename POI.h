
#ifndef EX1_POI_H
#define EX1_POI_H

#include <vector>
#include <string>
#include "Geometry.h"
#include "Entity.h"


class POI : public Entity {
    std::vector<std::string> accessibility;
    Geometry geometry;
public:
    POI(const std::string &id, const std::string &name, const std::string &description,
        const std::vector<std::string> &categoryTags, const std::vector<std::string> &accessibility,
        const Geometry &geometry);

    const std::vector<std::string> &getAccessibility() const;

    const Geometry &getGeometry() const;

};

#endif //EX1_POI_H
