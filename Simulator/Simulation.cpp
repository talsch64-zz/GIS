#include "Simulation.h"

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

Simulation::Simulation() : requestsFileParser(std::make_unique<RequestsFileParser>()) {
}

void Simulation::loadNavigationRequests(std::filesystem::path requestsPath) {
    requests = requestsFileParser->parse(requestsPath);
}

//Simulation::NavigationTasksManager::NavigationTasksManager() {}

//NavigationTask Simulation::NavigationTasksManager::createNextTask() {
//
////    return NavigationTask();
//}
