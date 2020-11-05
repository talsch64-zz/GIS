//
// Created by Tal on 05/11/2020.
//

#include "Junction.h"

Junction::Junction(const std::string &id, const std::string &name, const std::string &description,
                   const std::vector<std::string> &categoryTags, const Coordinates &coordinates) : Entity(id, name,
                                                                                                          description,
                                                                                                          categoryTags),
                                                                                                   coordinates(
                                                                                                           coordinates) {}
