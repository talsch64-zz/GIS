//
// Created by Tal on 05/11/2020.
//

#ifndef EX1_ENTITYJSONPARSER_H
#define EX1_ENTITYJSONPARSER_H

#include "rapidjson.h"
#include "document.h"
#include "POI.h"

using namespace rapidjson;

class EntityJsonParser {
public:
    template<typename T>
    T parse(Document &doc);

private:

    POI parsePoi(Document &doc);
};

#endif //EX1_ENTITYJSONPARSER_H
