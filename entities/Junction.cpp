//
// Created by Tal on 05/11/2020.
//

#include "Junction.h"

#include <utility>


Junction::Junction(const std::string &id, const std::string &name, const std::string &description,
                   const std::vector<std::string> &categoryTags, std::vector<Coordinates> coordinates) : Entity(
        id, name, description, categoryTags), coordinates(std::move(coordinates)) {}
