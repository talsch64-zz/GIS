#ifndef GIS_CPP_GISCONTAINER_H
#define GIS_CPP_GISCONTAINER_H


#include <atomic>
#include <mutex>
#include "../Common/AbstractGIS.h"

class GISContainer {
    std::function<std::unique_ptr<AbstractGIS>()> factory;
    std::unique_ptr<AbstractGIS> gis = nullptr;
    const std::string mapFilepath;
    const std::string name;
    std::atomic<int> usageCount = 0; // how many threads are currently working with the map
    std::atomic<bool> closeMapEnabled = false; // when the final task on this gis is created this flag is turned on
    static std::mutex freeGisMutex;

public:
    GISContainer(const std::function<std::unique_ptr<AbstractGIS>()> &factory, const std::string &name,
                 const std::string &mapFilepath);

    std::unique_ptr<AbstractGIS> &getGIS();

    const std::string &getName() const;

    void enableCloseMap();

    void incrementUsageCount();

    void decrementUsageCount();
};


#endif //GIS_CPP_GISCONTAINER_H
