#include "NavigationContainer.h"

NavigationContainer::NavigationContainer(const std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS&)> &factory,
                                         const std::string &name) : factory(factory), name(name) {}
