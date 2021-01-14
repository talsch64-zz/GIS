#ifndef GIS_CPP_GISCONTAINER_H
#define GIS_CPP_GISCONTAINER_H


#include <atomic>
#include "../Common/AbstractGIS.h"

class GISContainer {
    std::function<std::unique_ptr<AbstractGIS>()> factory;
    std::unique_ptr<AbstractGIS> gis = nullptr;
    inline static std::string mapFilepath;
    const std::string name;
    std::atomic<int> usageCount = 0; // how many threads are currently working with the map
    std::atomic<bool> closeMapEnabled = false; // when the final task on this gis is created this flag is turned on

public:
    static void setMapFilepath(const std::string &mapFilepath);

    GISContainer(const std::function<std::unique_ptr<AbstractGIS>()> &factory, const std::string &name);

    std::unique_ptr<AbstractGIS> &getGIS();

    const std::string &getName() const;

    //TODO: free gis when threads are done working on it
    void enableCloseMap();

    void incrementUsageCount();

    void decrementUsageCount();
};


#endif //GIS_CPP_GISCONTAINER_H
