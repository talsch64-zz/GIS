#include "Restrictions.h"

Restrictions::Restrictions() {
}

Restrictions::Restrictions(const char *restrictions) {
    //TODO: merge restrictions
    std::string res(restrictions);
    if (res == "highway") {
        this->restrictions.push_back(res);
    }
}

std::vector<std::string> Restrictions::get() const {
    return restrictions;
}
