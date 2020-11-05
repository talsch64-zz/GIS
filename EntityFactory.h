//
// Created by Tal on 05/11/2020.
//

#ifndef EX1_ENTITYFACTORY_H
#define EX1_ENTITYFACTORY_H

#include "rapidjson.h"
#include "document.h"
#include "POI.h"

using namespace rapidjson;

class EntityFactory {

    POI parsePoi(Document &doc);

public:
    template<typename T>
    T parseJsonEntity(Document &doc);
};

#endif //EX1_ENTITYFACTORY_H
