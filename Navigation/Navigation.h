#pragma once 

#include "../Common/NavigationGIS.h"
#include "../Common/AbstractNavigation.h"
#include "../Common/AbstractRoutes.h"
#include "Routes.h"
#include <memory>

/**
 * @brief Navigation class
 * 
 */
class Navigation_098765432 : public AbstractNavigation {
public:
    /**
     * @brief Construct a new Navigation object
     * 
     * @param gis - Note: you get a const ref of NavigationGIS
     */
    Navigation_098765432(const NavigationGIS& gis);

    /**
     * @brief Get the Routes object
     * 
     * @param start 
     * @param end 
     * @return Routes 
     */
    std::unique_ptr<AbstractRoutes> getRoutes(const Coordinates& start, const Coordinates& end) const override;
};

