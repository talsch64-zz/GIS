#include "EntityJsonParser.h"

template<typename T>
T EntityJsonParser::parse(Document &doc) {
    std::string type = doc["type"].GetString();
    if (type == "POI") {
        return parsePoi(doc);
    } else {
        return nullptr;
    }
}

POI EntityJsonParser::parsePoi(Document &doc) {
    //TODO: handle exception
    std::string id = doc["id"].GetString();
    std::string name = doc["name"].GetString();
    std::string description = doc["description"].GetString();
    std::vector<std::string> categoryTags;
    std::vector<std::string> accessibility;
    std::vector<Coordinates> coordinates;

    for (SizeType i = 0; i < doc["category_tags"].Size(); i++) {
        categoryTags.push_back(doc["category_tags"][i].GetString());
    }
    Geometry geometry(coordinates);
    POI poi(id, name, description, categoryTags, accessibility, geometry);
    return poi;
}