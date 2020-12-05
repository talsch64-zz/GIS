#include "Geometry.h"
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"

Geometry::Geometry(): geometryJsonSerializer(std::make_shared<GeometryJsonSerializer>()) {}

Geometry::~Geometry() {}



