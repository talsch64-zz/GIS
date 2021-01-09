#ifndef GIS_CPP_SIMULATION_H
#define GIS_CPP_SIMULATION_H

#include <vector>
#include <filesystem>
#include "../Common/AbstractGIS.h"
#include "../Common/AbstractNavigation.h"
#include "GISContainer.h"
#include "NavigationContainer.h"
#include "NavigationTask.h"
#include "RequestsFileParser.h"

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
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>> results;
    std::unique_ptr<RequestsFileParser> requestsFileParser;

    class NavigationTasksManager {
        static int nextGisIndex;
        static int nextNavigationIndex;
        static int nextRequestIndex;

//        NavigationTasksManager();
//        NavigationTask createNextTask();

    };


    Simulation();

public:
    static Simulation &getInstance() {
        static Simulation instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    Simulation(Simulation const &) = delete;

    void operator=(Simulation const &) = delete;

    void addGisFactory(std::function<std::unique_ptr<AbstractGIS>()> gisFactory);

    void
    addNavigationFactory(std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS &)> navigationFactory);

    void setNextName(std::string name);

    void loadNavigationRequests(std::filesystem::path requestsPath);
};


#endif //GIS_CPP_SIMULATION_H
