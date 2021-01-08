#include "Simulation.h"

void Simulation::addGis(std::unique_ptr<AbstractGIS> gis) {
    gisVec.push_back(std::move(gis));
}

void Simulation::addNavigation(std::unique_ptr<AbstractNavigation> navigation) {
    navigationVec.push_back(std::move(navigation));
}
