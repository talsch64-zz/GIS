#ifndef GIS_CPP_SIMULATION_H
#define GIS_CPP_SIMULATION_H

#include <vector>
#include "../Common/AbstractGIS.h"
#include "../Common/AbstractNavigation.h"
#include "GISContainer.h"
#include "NavigationContainer.h"

class Simulation {
private:
    std::vector<std::unique_ptr<GISContainer>> gisContainers;
    std::vector<std::unique_ptr<NavigationContainer>> navigationContainers;
    std::string nextName;

    Simulation() = default;

public:
    static Simulation &getInstance() {
        static Simulation instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    Simulation(Simulation const &) = delete;

    void operator=(Simulation const &) = delete;

    void addGisFactory(std::function<std::unique_ptr<AbstractGIS>()> gisFactory);

    void
    addNavigationFactory(std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS &)> navigationFactory);

    void setNextName(std::string name);
};


#endif //GIS_CPP_SIMULATION_H
