#ifndef GIS_CPP_NAVIGATIONCONTAINER_H
#define GIS_CPP_NAVIGATIONCONTAINER_H


#include "../Common/AbstractNavigation.h"

/**
 * class NavigationContainer holds Navigation factory and the class name;
 */
class NavigationContainer {
    std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS &)> factory;
    const std::string name;

public:
    NavigationContainer(const std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS &)> &factory,
                        const std::string &name);

    std::unique_ptr<AbstractNavigation> getNavigation(const NavigationGIS &navigationGIS);

    const std::string &getName() const;
};


#endif //GIS_CPP_NAVIGATIONCONTAINER_H
