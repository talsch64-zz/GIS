
#ifndef EX1_WAY_H
#define EX1_WAY_H

#include "Entity.h"
#include "../GISdefs.h"

class Way: public Entity {
    std::string from;
    std::string to;
    std::vector<Coordinates> curves;
    std::string direction;
    std::vector<std::string> categoryTags;
    int speedLimit;
    bool tollRoad;
    std::vector<std::string> restricted;

public:
    Way(const std::string &id, const std::string &name, const std::string &description,
        const std::vector<std::string> &categoryTags, const std::string &from, const std::string &to,
        const std::vector<Coordinates> &curves, const std::string &direction,
        const std::vector<std::string> &categoryTags1, int speedLimit, bool tollRoad,
        const std::vector<std::string> &restricted);
};

#endif //EX1_WAY_H
