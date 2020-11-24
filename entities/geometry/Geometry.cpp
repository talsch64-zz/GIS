#include "Geometry.h"
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"

Geometry::Geometry(std::string _type): geometryJsonSerializer(new GeometryJsonSerializer()), type(std::move(_type)) {}

Geometry::~Geometry() {
    delete(geometryJsonSerializer);
}

const std::string &Geometry::getType() const {
    return type;
}


