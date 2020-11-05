

#include "GIS.h"
#include "rapidjson.h"
#include "document.h"
using namespace rapidjson;
std::size_t clear() {}
std::vector<EntityId> loadMapFile(const std::string& filename) {
    Document document;
    document.Parse(filename.c_str());
    if (!document.IsArray()) {
        //TODO: handle errors
        throw "Map is not an array";
    }

    for(SizeType i = 0; i < document.Size(); i++) {
        a=document[i].GetInt()
    }




}
std::size_t saveMapFile(const std::string& filename) {}
std::vector<EntityId> getEntities(const std::string& search_name) {}
std::vector<EntityId> getEntities(const std::string& search_name, const Coordinates&, Meters radius) {}
std::optional<Coordinates> getEntityClosestPoint(const EntityId&, const Coordinates&) {}
std::pair<Coordinates, EntityId> getWayClosestPoint(const Coordinates&) {}