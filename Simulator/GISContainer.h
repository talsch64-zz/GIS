#ifndef GIS_CPP_GISCONTAINER_H
#define GIS_CPP_GISCONTAINER_H


#include "../Common/AbstractGIS.h"

class GISContainer {
    std::function<std::unique_ptr<AbstractGIS>()> factory;
    std::unique_ptr<AbstractGIS> gis = nullptr;
    std::string name;

public:
    GISContainer(const std::function<std::unique_ptr<AbstractGIS>()> &factory, const std::string &name);
};


#endif //GIS_CPP_GISCONTAINER_H
