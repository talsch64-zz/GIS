#include <cstring>
#include "Restrictions.h"

Restrictions::Restrictions(const char *restrictions): restrictions(split(restrictions, " ,")) {}


std::unordered_set<std::string> Restrictions::split(const char *restrictions, const char* delimiter) {
    std::unordered_set<std::string> restrictionsSet;
    char restrictionsCopy[strlen(restrictions)];
    strcpy(restrictionsCopy, restrictions);
    char* restriction = strtok(restrictionsCopy, delimiter);
    while (restriction != nullptr) {
        restrictionsSet.insert(std::string(restriction));
        restriction = strtok (nullptr, delimiter);
    }
    return restrictionsSet;
}
