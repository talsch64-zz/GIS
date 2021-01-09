#include "Simulation.h"

Simulation::Simulation() : requestsFileParser(std::make_unique<RequestsFileParser>()) {
}

void Simulation::addGisFactory(std::function<std::unique_ptr<AbstractGIS>()> gisFactory) {
    std::unique_ptr<GISContainer> gisContainer = std::make_unique<GISContainer>(gisFactory, nextName);
    gisContainers.push_back(std::move(gisContainer));
}

void Simulation::addNavigationFactory(
        std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS &)> navigationFactory) {
    std::unique_ptr<NavigationContainer> navigationContainer = std::make_unique<NavigationContainer>(navigationFactory,
                                                                                                     nextName);
    navigationContainers.push_back(std::move(navigationContainer));
}

void Simulation::setNextName(std::string name) {
    nextName = name;
}

std::unique_ptr<GISContainer> &Simulation::getGISContainer(int index) {
    return gisContainers[index];
}

std::unique_ptr<NavigationContainer> &Simulation::getNavigationContainer(int index) {
    return navigationContainers[index];
}

NavigationRequest Simulation::getNavigationRequest(int index) {
    return requests[index];
}

void Simulation::startSimulation(std::unique_ptr<Registrar> &registrar) {
    requests = requestsFileParser->parse(registrar->getNavigationRequestsPath());

    results = std::make_unique<std::unique_ptr<AbstractRoutes>[]>(
            gisContainers.size() * navigationContainers.size() * requests.size());
    threads = std::make_unique<std::thread[]>(registrar->getNumThreads());

    for (int i = 0; i < registrar->getNumThreads(); i++) {
        threads[i] = std::thread(&Simulation::navigationThread, this);
    }
    std::unique_lock<std::mutex> lck(taskMutex);

}

void Simulation::navigationThread() {

}
