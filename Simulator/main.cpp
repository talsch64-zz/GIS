#include "../Common/Restrictions.h"
#include "Simulation.h"
#include <cstdlib>
#include <dlfcn.h>
#include <string>
#include <filesystem>
#include <iostream>
#include <set>
#include <cassert>
#include <unistd.h>
#include <getopt.h>
#include "Registrar.h"

int main(int argc, char *argv[]) {
    std::unique_ptr<Registrar> registrar = std::make_unique<Registrar>();
    registrar->parseCommandLineArguments(argc, argv);
    registrar->loadSharedLibraries();
    Simulation &simulation = Simulation::getInstance();
    simulation.startSimulation(registrar);

    registrar->unloadSharedLibraries();
    return EXIT_SUCCESS;
}
