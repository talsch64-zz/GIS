//
// Created by Tal on 05/11/2020.
//

#include "EntityFactory.h"

template<typename T>
T EntityFactory::parseJsonEntity(Document &doc) {
    std::string type = doc["type"].GetString();
    if (type == "POI") {
        return parsePoi(doc);
    }
    else {
        return nullptr;
    }
}

POI& parsePoi(Document &doc) {
    //TODO: handle exception
    std::string id = doc["id"].GetString();
    std::string name = doc["name"].GetString();
    std::string description = doc["description"].GetString();
    std::vector<std::string> categoryTags;

    for (SizeType i = 0; i < doc["category_tags"].Size(); i++) {
        categoryTags.push_back(doc["category_tags"][i].GetString());
    }
     POI poi(id,name,description,categoryTags, nullptr, nullptr);
}