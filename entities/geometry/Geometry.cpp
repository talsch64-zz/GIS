#include "Geometry.h"
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"

Geometry::Geometry(): geometryJsonSerializer(new GeometryJsonSerializer()) {}

Geometry::~Geometry() {
    delete(geometryJsonSerializer);
}


