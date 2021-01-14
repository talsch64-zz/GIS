#pragma once

#include "Navigation_315524694.h"

class NavigationProvider {
public:
        static auto getNavigation(const NavigationGIS& navigation_gis) {
                return Navigation_315524694{navigation_gis};
        }
};
