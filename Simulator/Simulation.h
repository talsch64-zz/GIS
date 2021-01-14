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
#include "TaskResult.h"
#include <iostream>


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
    std::unique_ptr<std::unique_ptr<TaskResult>[]> results = nullptr;
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

    /**
     * @brief clear all data members that holds symbols related to Navigation.so or GIS.so (called before dlclose)
     */
    void clear();

    Simulation(Simulation const &) = delete;

    void operator=(Simulation const &) = delete;

    void startSimulation(std::unique_ptr<Registrar> &registrar);

    void addGisFactory(std::function<std::unique_ptr<AbstractGIS>()> gisFactory);

    void
    addNavigationFactory(std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS &)> navigationFactory);

    void setNextName(std::string name);

    std::unique_ptr<GISContainer> &getGISContainer(int index);

    std::unique_ptr<NavigationContainer> &getNavigationContainer(int index);

    const NavigationRequest &getNavigationRequest(int index);

    /**
     * Get a reference to the array element of the result,
     * corresponding to a GIS, Navigation algorithm and request combination
     * @param gisIndex
     * @param navigationIndex
     * @param requestIndex
     * @return result
     */
    std::unique_ptr<TaskResult> &getResult(int gisIndex, int navigationIndex, int requestIndex);

    void setResult(int gisIndex, int navigationIndex, int requestIndex, std::unique_ptr<TaskResult> result);


    //TODO delete!!!!
    void printResults() {
        std::cout << std::endl;
        for (int i = 0; i < requests.size(); i++) {
            for (int j = 0; j < navigationContainers.size(); j++) {
                for (int k = 0; k < gisContainers.size(); k++) {
                    auto &result = getResult(k, j, i);
                    std::cout << "GIS: " << gisContainers[k]->getName() << ", Navigation: " << navigationContainers[j]->getName() << ", request: " << i << std::endl;
                    std::cout << "=== shortestDistance route ===" << std::endl << "Length: "
                              << static_cast<double>(result->getRoutes()->shortestDistance().totalLength())
                              << ", Time: "
                              << static_cast<double> (result->getRoutes()->shortestDistance().estimatedDuration())
                              << ", Usage:" << result->getGisUsageCount()
                              << std::endl << std::endl;

                    std::cout << "=== shortestTime route ===" << std::endl << "Length: "
                              << static_cast<double>(result->getRoutes()->shortestTime().totalLength())
                              << ", Time: "
                              << static_cast<double>(result->getRoutes()->shortestTime().estimatedDuration())
                              << ", Usage:" << result->getGisUsageCount()
                              << std::endl << std::endl;
                }
            }
        }
    }

private:

    /**
     * @brief the function where threads execute navigation tasks
     */
    void navigationThread();


    /**
     * @brief execute a task (task = Navigation x GIS x request)
     * @param task the task to execute
     * @return std::unique_ptr<TaskResult>
     */
    std::unique_ptr<TaskResult> executeTask(const NavigationTask &task);
};


#endif //GIS_CPP_SIMULATION_H
