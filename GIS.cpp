

#include <vector>
#include <optional>
#include "rapidjson/document.h"
#include "GIS.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

std::size_t GIS::clear() {
    return 0;
}

std::vector<EntityId> GIS::loadMapFile(const std::string &filename) {
    rapidjson::Document document;
    std::vector<EntityId> entityIds;
    std::vector<char> *fileContent = readJsonFile(filename);
    rapidjson::ParseResult ok = document.Parse(fileContent->data());

    delete fileContent;
//    if (!ok) {
//        throw std::runtime_error("JSON parse error");
//    }
    if (!document.IsArray()) {
        //TODO: handle errors
        throw std::runtime_error("Map is not an array");
    }

    for (auto &jsonEntity : document.GetArray()) {
        try {
            std::unique_ptr<Entity> entity = entityJsonParser.parse(jsonEntity);
            EntityId entityId = entity->getId();
            entities.emplace(entityId, std::move(entity));
            entityIds.push_back(entityId);

        }
        catch (const std::runtime_error &e) {
        }
    }
    return entityIds;
}

std::size_t GIS::saveMapFile(const std::string &filename) {
    rapidjson::Document doc;
    doc.SetArray();
    std::size_t size = 0;
    for (auto &entityPair : entities) {
        rapidjson::Value entityVal = entityPair.second->toJson(doc.GetAllocator());
        doc.PushBack(entityVal, doc.GetAllocator());
        size++;
    }
    jsonFileWriter.write(doc, filename);
    return size;
}

std::vector<EntityId> GIS::getEntities(const std::string &search_name) { return std::vector<EntityId>(); }

std::vector<EntityId>
GIS::getEntities(const std::string &search_name, const Coordinates &, Meters radius) { return std::vector<EntityId>(); }

std::optional<Coordinates> GIS::getEntityClosestPoint(const EntityId &, const Coordinates &) {
    Coordinates coord(Longitude(0), Latitude(0));
    return coord;
}

std::pair<Coordinates, EntityId> GIS::getWayClosestPoint(const Coordinates &) {
    Coordinates coord(Longitude(0), Latitude(0));
    std::pair<Coordinates, EntityId> p(coord, "something");
    return p;
}

std::vector<char> *GIS::readJsonFile(const std::string &filePath) {
    std::ifstream ifile(filePath, std::ios::ate);
    if (!ifile) {
        throw std::runtime_error("Could not open file " + filePath);
    }

    std::streamsize size = ifile.tellg();
    ifile.seekg(0, std::ios::beg);

    auto *buffer = new std::vector<char>(size);
    if (!ifile.read(buffer->data(), size)) {
        //TODO: figure out why this happens, despite a successful read
        std::cerr << "Could not read file " << filePath << '\n';
    }

    ifile.close();

    return buffer;
}