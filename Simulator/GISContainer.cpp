#include "GISContainer.h"

GISContainer::GISContainer(const std::function<std::unique_ptr<AbstractGIS>()> &factory, const std::string &name,
                           const std::string &mapFilepath)
        : factory(factory), name(name), mapFilepath(mapFilepath) {}

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

void GISContainer::enableCloseMap() {
    closeMapEnabled = true;
}

void GISContainer::incrementUsageCount() {
    usageCount++;
}

void GISContainer::decrementUsageCount() {
    usageCount--;
    if (closeMapEnabled && usageCount == 0) {
        gis.reset(); // No need for gis anymore. No need for locks either!
    }
}



