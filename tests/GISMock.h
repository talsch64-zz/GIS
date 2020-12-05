#include "../GIS.h"

class GISMock : public GIS
{
public:
    bool addEntity(std::unique_ptr<Entity> entity);
};
