#include <cstring>
#include "Restrictions.h"

Restrictions::Restrictions(const char *restrictions): restrictions(split(restrictions, " ,")) {}


std::vector<std::string> Restrictions::split(const char *restrictions, const char* delimiter) {
    std::vector<std::string> restrictionsVector;
    char restrictionsCopy[strlen(restrictions)];
    strcpy(restrictionsCopy, restrictions);
    char* restriction = strtok(restrictionsCopy, delimiter);
    while (restriction != nullptr) {
        restrictionsVector.push_back(std::string(restriction));
        restriction = strtok (nullptr, delimiter);
    }
    return restrictionsVector;
}

const std::vector<std::string> &Restrictions::getRestrictions() const {
    return restrictions;
}

Restrictions::Restrictions(): restrictions(std::vector<std::string>()) {}
