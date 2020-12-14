#include <cstring>
#include "Restrictions.h"

Restrictions::Restrictions(const char *restrictions): restrictions(splitRestrictions(restrictions)) {}


std::unordered_set<std::string> Restrictions::splitRestrictions(const char *restrictions) {
    std::unordered_set<std::string> restrictionsSet;
    char restrictionsCopy[strlen(restrictions)];
    strcpy(restrictionsCopy, restrictions);
    char* restriction = strtok(restrictionsCopy, " ,");
    while (restriction != nullptr) {
        restrictionsSet.insert(std::string(restriction));
        restriction = strtok (nullptr, " ,");
    }
    return restrictionsSet;
}
