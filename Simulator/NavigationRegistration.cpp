#include "../Common/NavigationRegistration.h"
#include "../Common/AbstractGIS.h" // For demo only
#include "../Common/NavigationGIS.h"
#include "Simulation.h"
#include "../GIS/GIS.h"
#include <iostream> // For demo only

NavigationRegistration::NavigationRegistration(
        std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS &)> factory) {
    Simulation &sim = Simulation::getInstance();
    sim.addNavigationFactory(factory);
}
