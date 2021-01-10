#include "GISContainer.h"

GISContainer::GISContainer(const std::function<std::unique_ptr<AbstractGIS>()> &factory, const std::string &name)
        : factory(factory), name(name) {}

std::unique_ptr<AbstractGIS> &GISContainer::getGIS() {
    if (gis == nullptr) {
        gis = factory();
        gis->loadMapFile(mapFilepath);
    }
    return gis;
}

const std::string &GISContainer::getName() const {
    return name;
}

void GISContainer::setMapFilepath(const std::string &mapFilepath) {
    GISContainer::mapFilepath = mapFilepath;
}
