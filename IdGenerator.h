
#ifndef EX1_IDGENERATOR_H
#define EX1_IDGENERATOR_H

#include <string>
#include "GISdefs.h"

class IdGenerator {
    int count = 1;
public:
    EntityId generateId() {
        return EntityId("G" + std::to_string(count++));
    }
};

#endif //EX1_IDGENERATOR_H
