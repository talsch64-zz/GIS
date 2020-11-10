

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

    if (!ok) {
        throw std::runtime_error("JSON parse error");
    }

    if (!document.IsArray()) {
        //TODO: handle errors
        throw std::runtime_error("Map is not an array");
    }

    for (auto &jsonEntity : document.GetArray()) {
        try {
            Entity entity = entityJsonParser.parse(jsonEntity);
            entities.push_back(entity);
            entityIds.push_back(entity.getId());
        }
        catch (const std::runtime_error &e) {
        }
    }
    return entityIds;
}

std::size_t GIS::saveMapFile(const std::string &filename) {
    return 0;
}

std::vector<EntityId> GIS::getEntities(const std::string &search_name) { return std::vector<EntityId>(); }

std::vector<EntityId>
GIS::getEntities(const std::string &search_name, const Coordinates &, Meters radius) { return std::vector<EntityId>(); }

std::optional<Coordinates> GIS::getEntityClosestPoint(const EntityId &, const Coordinates &) {
    Coordinates coord(0, 0);
    return coord;
}

std::pair<Coordinates, EntityId> GIS::getWayClosestPoint(const Coordinates &) {
    Coordinates coord(0, 0);
    std::pair<Coordinates, EntityId> p(coord, "something");
    return p;
}

std::vector<char> *GIS::readJsonFile(std::string filePath) {
    std::ifstream ifile(filePath, std::ios::ate);
    if (!ifile) {
        throw std::runtime_error("Could not open file " + filePath);
    }

    std::streamsize size = ifile.tellg();
    ifile.seekg(0, std::ios::beg);

    std::vector<char> *buffer = new std::vector<char>(size);
    if (!ifile.read(buffer->data(), size)) {
        //TODO: figure out why this happens, despite a successful read
        std::cerr << "Could not read file " << filePath << '\n';
    }

    ifile.close();

    return buffer;
}