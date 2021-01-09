#ifndef GIS_CPP_SIMULATION_H
#define GIS_CPP_SIMULATION_H

#include <vector>
#include <filesystem>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "../Common/AbstractGIS.h"
#include "../Common/AbstractNavigation.h"
#include "GISContainer.h"
#include "NavigationContainer.h"
#include "NavigationTask.h"
#include "RequestsFileParser.h"
#include "Registrar.h"
#include "NavigationTasksManager.h"

/**
 * @brief class Simulation simulates the various navigation and gis .so files on a given map file
 * and navigation requests and analyzes the results.
 * Implemented as a Singleton.
 */
class Simulation {
    std::vector<std::unique_ptr<GISContainer>> gisContainers;
    std::vector<std::unique_ptr<NavigationContainer>> navigationContainers;
    std::string nextName;
    std::vector<NavigationRequest> requests;
    std::unique_ptr<std::unique_ptr<AbstractRoutes>[]> results = nullptr;
    std::unique_ptr<RequestsFileParser> requestsFileParser;
    std::mutex taskMutex;
    std::unique_ptr<std::thread[]> threads = nullptr;
    std::unique_ptr<NavigationTasksManager> taskManager = nullptr;

    Simulation();

public:
    static Simulation &getInstance() {
        static Simulation instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    Simulation(Simulation const &) = delete;

    void operator=(Simulation const &) = delete;

    void startSimulation(std::unique_ptr<Registrar> &registrar);

    void addGisFactory(std::function<std::unique_ptr<AbstractGIS>()> gisFactory);

    void
    addNavigationFactory(std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS &)> navigationFactory);

    void setNextName(std::string name);

    std::unique_ptr<GISContainer> &getGISContainer(int index);

    std::unique_ptr<NavigationContainer> &getNavigationContainer(int index);

    NavigationRequest getNavigationRequest(int index);

    void navigationThread();

    /**
     * Get a reference to the array element of the result,
     * corresponding to a GIS, Navigation algorithm and request combination
     * @param gisIndex
     * @param navigationIndex
     * @param requestIndex
     * @return result
     */
    std::unique_ptr<AbstractRoutes> &getResult(int gisIndex, int navigationIndex, int requestIndex);
};


#endif //GIS_CPP_SIMULATION_H
