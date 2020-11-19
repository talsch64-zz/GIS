#ifndef EX1_ENTITYJSONSERIALIZER_H
#define EX1_ENTITYJSONSERIALIZER_H

#include <rapidjson/document.h>
#include <memory>
#include <vector>
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

    rapidjson::Value
    setString(rapidjson::Value json, const rapidjson::GenericStringRef<char>& name, std::string str,
              rapidjson::Document::AllocatorType &allocator);

    rapidjson::Value
    setStringVector(rapidjson::Value json, rapidjson::GenericStringRef<char> name, const std::vector<std::string>& vec,
                    rapidjson::Document::AllocatorType &allocator);
};

#endif //EX1_ENTITYJSONSERIALIZER_H
