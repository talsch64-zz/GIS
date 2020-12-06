#include "../GIS.h"

class GISMock : public GIS
{
public:
    bool addEntity(std::unique_ptr<Entity> entity);
    const std::unordered_map<EntityId, std::unique_ptr<Entity>> &getEntityMap();
    TopologicalSearch *getTopologicalSearch();
};
