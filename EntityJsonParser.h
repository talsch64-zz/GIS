//
// Created by Tal on 05/11/2020.
//

#ifndef EX1_ENTITYJSONPARSER_H
#define EX1_ENTITYJSONPARSER_H

#include <rapidjson/document.h>
#include "POI.h"

class EntityJsonParser {
public:
    Entity parse(rapidjson::Value &doc);

private:

    POI parsePoi(rapidjson::Value &doc);
};

#endif //EX1_ENTITYJSONPARSER_H
