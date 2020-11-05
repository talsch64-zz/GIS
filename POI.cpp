//
// Created by Tal on 05/11/2020.
//

#include "POI.h"

POI::POI(const std::string &id, const std::string &name, const std::string &description,
         const std::vector<std::string> &categoryTags, const std::vector<std::string> &accessibility,
         const Geometry &geometry) : Entity(id, name, description, categoryTags), accessibility(accessibility),
                                     geometry(geometry) {}

const std::vector<std::string> &POI::getAccessibility() const {
    return accessibility;
}

const Geometry &POI::getGeometry() const {
    return geometry;
}
