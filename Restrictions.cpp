#include "Restrictions.h"
#include "Utils.h"

Restrictions::Restrictions(const char *restrictions) : restrictions(Utils::split(restrictions, " ,")) {}


const std::vector<std::string> &Restrictions::getRestrictions() const {
    return restrictions;
}

Restrictions::Restrictions() : restrictions(std::vector<std::string>()) {}
