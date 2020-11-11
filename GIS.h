
#ifndef EX1_GIS_H
#define EX1_GIS_H

#include <utility>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include "entities/EntityJsonParser.h"

class GIS {
    std::unordered_map<EntityId, Entity*> entities;
    EntityJsonParser entityJsonParser;

public:
    std::size_t clear();

    std::vector<EntityId> loadMapFile(const std::string &filename);

    std::size_t saveMapFile(const std::string &filename);

    std::vector<EntityId> getEntities(const std::string &search_name);

    std::vector<EntityId> getEntities(const std::string &search_name, const Coordinates &, Meters radius);

    std::optional<Coordinates> getEntityClosestPoint(const EntityId &, const Coordinates &);

    std::pair<Coordinates, EntityId> getWayClosestPoint(const Coordinates &);

private:
    std::vector<char> *readJsonFile(const std::string &filePath);
};

#endif //EX1_GIS_H
