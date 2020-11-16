//
// Created by Tal on 05/11/2020.
//

#ifndef EX1_JUNCTION_H
#define EX1_JUNCTION_H

#include "Entity.h"
#include "../GISdefs.h"

class Junction: public Entity {
    std::vector<Coordinates> coordinates;
public:
    Junction(const std::string &id, const std::string &name, const std::string &description,
             const std::vector<std::string> &categoryTags, std::vector<Coordinates> coordinates);

};

#endif //EX1_JUNCTION_H
