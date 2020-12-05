#include "GISMock.h";

bool GISMock::addEntity(std::unique_ptr<Entity> entity)
{
    GIS::addEntity(std::move(entity));
}