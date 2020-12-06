
#ifndef EX1_IDGENERATOR_H
#define EX1_IDGENERATOR_H

#include <string>
#include "GISNamedTypes.h"


/// IdGenerator generates ids for entities without ids
class IdGenerator {
    int count = 1;
public:
    EntityId generateId() {
        return EntityId("G" + std::to_string(count++));
    }
};

#endif //EX1_IDGENERATOR_H
