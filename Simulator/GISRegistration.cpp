#include "../Common/GISRegistration.h"
#include "Simulation.h"

GISRegistration::GISRegistration(std::function<std::unique_ptr<AbstractGIS>()> func) {
    Simulation &sim = Simulation::getInstance();
    sim.addGisFactory(func);
}
