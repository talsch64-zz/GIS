#ifndef GIS_CPP_GISCONTAINER_H
#define GIS_CPP_GISCONTAINER_H


#include <atomic>
#include <mutex>
#include "../Common/AbstractGIS.h"

/**
 * class GISContainer
 *  Data Members:
 *      GIS factory
 *      GIS - may be uninitialized. Loaded only when needed and unloaded when all the threads finished working on it.
 *      name - GIS class name
 *      mapFilepath - the path of the map file
 *      usageCount - indicates how many threads are working concurrently on the gis
 *      closeMapEnabled - flag indicates the the GIS can be unloaded after usageCount drops to zero.
 *      freeGisMutex - mutex for unloading the GIS
 * The container holds a loaded GIS for the shortest time possible - The simulation load the gis and utilize it for all the requests and Navigation
 * combinations and unloads it right after all the last combination returned a result.
 *
 */
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

    /**
     * @brief turn on the closeMapEnabled flag. This function is called by TaskManager when the last NavigationTask is created.
     */
    void enableCloseMap();

    void incrementUsageCount();

    /*
     * @brief decrements usageCount and unloads gis if closedEnabled is turned on and the usage count is drops to zero
     */
    void decrementUsageCount();
};


#endif //GIS_CPP_GISCONTAINER_H
