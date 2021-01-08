#ifndef GIS_CPP_SIMULATION_H
#define GIS_CPP_SIMULATION_H

#include <vector>
#include "../Common/AbstractGIS.h"
#include "../Common/AbstractNavigation.h"

class Simulation {
private:
    std::vector<std::unique_ptr<AbstractGIS>> gisVec;
    std::vector<std::unique_ptr<AbstractNavigation>> navigationVec;

    Simulation() = default;

public:
    static Simulation &getInstance() {
        static Simulation instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    Simulation(Simulation const &) = delete;

    void operator=(Simulation const &) = delete;

    void addGis(std::unique_ptr<AbstractGIS> gis);

    void addNavigation(std::unique_ptr<AbstractNavigation> navigation);
};


#endif //GIS_CPP_SIMULATION_H
