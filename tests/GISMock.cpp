#include "GISMock.h";

bool GISMock::addEntity(std::unique_ptr<Entity> entity)
{
    GIS::addEntity(std::move(entity));
}

const std::unordered_map<EntityId, std::unique_ptr<Entity>> &GISMock::getEntityMap()
{
    return entities;
}

TopologicalSearch *GISMock::getTopologicalSearch()
{
    return topologicalSearch.get();
}