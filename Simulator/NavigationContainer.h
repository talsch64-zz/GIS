#ifndef GIS_CPP_NAVIGATIONCONTAINER_H
#define GIS_CPP_NAVIGATIONCONTAINER_H


#include "../Common/AbstractNavigation.h"

class NavigationContainer {
    std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS &)> factory;
    const std::string name;

public:
    NavigationContainer(const std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS &)> &factory,
                        const std::string &name);

    std::unique_ptr<AbstractNavigation> getNavigation(const NavigationGIS &navigationGIS);
};


#endif //GIS_CPP_NAVIGATIONCONTAINER_H
