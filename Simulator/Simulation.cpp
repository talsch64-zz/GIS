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
    GISContainer::setMapFilepath(registrar->getMapFilePath());
    taskManager = std::make_unique<NavigationTasksManager>(gisContainers.size(), navigationContainers.size(),
                                                           requests.size());
    results = std::make_unique<std::unique_ptr<TaskResult>[]>(
            gisContainers.size() * navigationContainers.size() * requests.size());
    threads = std::make_unique<std::thread[]>(registrar->getNumThreads());

    //TODO: case of 1 thread
    for (int i = 0; i < registrar->getNumThreads(); i++) {
        threads[i] = std::thread(&Simulation::navigationThread, this);
    }

    for (int i = 0; i < registrar->getNumThreads(); i++) {
        threads[i].join();
    }
}

void Simulation::navigationThread() {
    std::unique_lock<std::mutex> lck(taskMutex);
    bool con = true;
    while (con) {
        std::unique_ptr<NavigationTask> task;
        lck.lock();
        con = taskManager->hasTask();
        if (con) {
            task = taskManager->getNextTask();
        }
        lck.unlock();
        //TODO wrap inside a function "executeTask"
        if (con) {
            auto &navigation = task->getNavigation();
            auto req = task->getRequest();
            auto &result = getResult(task->getGisIndex(), task->getNavigationIndex(), task->getRequestIndex());
            result->setRoutes(navigation->getRoutes(req.getFrom(), req.getTo()));
            auto &routes = result->getRoutes();
            bool validRoutes = routes->isValid();
            if (validRoutes) {
                auto &shortestDistanceRoute = routes->shortestDistance();
                auto &shortestTimeRoute = routes->shortestTime();
                auto &start = shortestTimeRoute.getWayStartPoint();
                auto &end = shortestTimeRoute.getWayEndPoint();
                result->setShortestDistanceValid(task->getValidator()->validateRoute(start, end, shortestDistanceRoute));
                result->setShortestTimeValid(task->getValidator()->validateRoute(start, end, shortestTimeRoute));
            }
            result->setGisUsageCount(task->getNavigationGis()->getUsageCounter());
        }
    }
}

std::unique_ptr<TaskResult> &Simulation::getResult(int gisIndex, int navigationIndex, int requestIndex) {
    int index = gisIndex * navigationContainers.size() * requests.size() +
                navigationIndex * requests.size() +
                requestIndex;
    return results[index];
}
