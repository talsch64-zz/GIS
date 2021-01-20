#include "../Common/Restrictions.h"
#include "Simulation.h"
#include <cstdlib>
#include "Registrar.h"

int main(int argc, char *argv[]) {
    Simulation &simulation = Simulation::getInstance();
    auto &registrar = simulation.getRegistrar();
    registrar->parseCommandLineArguments(argc, argv);
    registrar->loadSharedLibraries();
    simulation.startSimulation();
    simulation.clear(); // clear all objects from the shared libraries
    registrar->unloadSharedLibraries();
    return EXIT_SUCCESS;
}
