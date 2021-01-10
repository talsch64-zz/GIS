#ifndef GIS_CPP_GISCONTAINER_H
#define GIS_CPP_GISCONTAINER_H


#include "../Common/AbstractGIS.h"

class GISContainer {
    std::function<std::unique_ptr<AbstractGIS>()> factory;
    std::unique_ptr<AbstractGIS> gis = nullptr;
    static std::string mapFilepath;
    const std::string name;
public:
    static void setMapFilepath(const std::string &mapFilepath);

    GISContainer(const std::function<std::unique_ptr<AbstractGIS>()> &factory, const std::string &name);

    std::unique_ptr<AbstractGIS> &getGIS();

    const std::string &getName() const;

    //TODO: free gis when threads are done working on it
};


#endif //GIS_CPP_GISCONTAINER_H
