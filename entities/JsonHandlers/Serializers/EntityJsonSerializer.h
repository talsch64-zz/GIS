#ifndef EX1_ENTITYJSONSERIALIZER_H
#define EX1_ENTITYJSONSERIALIZER_H

#include <rapidjson/document.h>
#include <memory>
#include "GeometryJsonSerializer.h"

class POI;

class Junction;

class Way;

class Entity;

class Geometry;

class EntityJsonSerializer {
    CoordinatesJsonSerializer coordinatesJsonSerializer;

public:
    rapidjson::Value toJson(POI *entity, rapidjson::Document::AllocatorType &allocator);

    rapidjson::Value toJson(Junction *entity, rapidjson::Document::AllocatorType &allocator);

    rapidjson::Value toJson(Way *entity, rapidjson::Document::AllocatorType &allocator);

private:
    rapidjson::Value entityToJson(Entity *entity, rapidjson::Document::AllocatorType &allocator);

    rapidjson::Value setGeometry(rapidjson::Value entityJson, Geometry *geometry,
                                 rapidjson::Document::AllocatorType &allocator);
};

#endif //EX1_ENTITYJSONSERIALIZER_H
