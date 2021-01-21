#include "GISContainer.h"
std::mutex GISContainer::freeGisMutex;

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
        //it's ok for this line to be called multiple times by multiple threads (just not concurrently, that's what the lock is for)
        {
            std::unique_lock<std::mutex> lock(freeGisMutex);
            gis.reset(); // No need for gis anymore.
        }
    }
}
