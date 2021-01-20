#include "gtest/gtest.h"
#include "../../Simulator/Simulation.h"
#include <fstream>
#include <cstring>
#include <string>


class SimulatorTest : public ::testing::Test {
    char *mapsDirectory = strdup("/home/student/repos/advanced-topics/advanced-ex1/tests/");
    char *requestsDirectory = strdup("/home/student/repos/advanced-topics/advanced-ex1/tests/Simulator/");
    char *navigationSoDirectory = strdup("/home/student/repos/advanced-topics/advanced-ex1/Navigation/old-obuntu-so");
    char *gisSoDirectory = strdup("/home/student/repos/advanced-topics/advanced-ex1/GIS/old-obuntu-so");


public:
    SimulatorTest() {
    }

    void SetUp() override {

    }

    void startSimulation(int numThreads, std::string mapFileName, std::string requestsFileName) {
        Simulation &simulation = Simulation::getInstance();
        char *mapPath = (char *) malloc(strlen(mapsDirectory) + mapFileName.size() + 1);
        strcpy(mapPath, mapsDirectory);
        strcat(mapPath, mapFileName.c_str());
        char *requestsPath = (char *) malloc(strlen(requestsDirectory) + requestsFileName.size() + 1);
        strcpy(requestsPath, requestsDirectory);
        strcat(requestsPath, requestsFileName.c_str());
        char *numThreadsChars = (char *) malloc(3);
        sprintf(numThreadsChars, "%d", numThreads);
        char *numThreadsTitle = strdup("-num_threads");
        char *navigationTitle = strdup("-navigation");
        char *gisTitle = strdup("-gis");
        char *mapFileTitle = strdup("-map_file");
        char *navigationRequestsTitle = strdup("-navigation_requests");
        char *argv[] = {
                numThreadsTitle, numThreadsChars, navigationTitle, navigationSoDirectory,
                gisTitle, gisSoDirectory, mapFileTitle, mapPath, navigationRequestsTitle, requestsPath
        };
        int argc = 11;

        auto &registrar = simulation.getRegistrar();
        registrar->parseCommandLineArguments(argc, argv);
        registrar->loadSharedLibraries();
        simulation.startSimulation();

        free(numThreadsChars);
        free(numThreadsTitle);
        free(navigationTitle);
        free(gisTitle);
        free(mapFileTitle);
        free(navigationRequestsTitle);
        free(mapPath);
        free(requestsPath);
    }

    void TearDown()
    override {
        Simulation &simulation = Simulation::getInstance();
        auto &registrar = simulation.getRegistrar();
        simulation.clear(); // clear all objects from the shared libraries
        registrar->unloadSharedLibraries();
    }

    std::vector<std::string> getStrangeResultsLines() {
        std::vector<std::string> lines;
        std::ifstream infile("strange_GIS_results.log");
        std::string line;
        while (std::getline(infile, line)) {
            lines.push_back(line);
        }
        lines.pop_back();
        return lines;
    }

    std::vector<std::string> splitLineToParts(std::string line) {
        std::vector<std::string> arr;
        char *cstr = const_cast<char *>(line.c_str());
        std::string sep = ", ";
        char *current = std::strtok(cstr, sep.c_str());
        while (current != nullptr) {
            arr.emplace_back(current);
            current = std::strtok(nullptr, sep.c_str());
        }
        return arr;
    }

    virtual ~SimulatorTest() {
        free(mapsDirectory);
        free(requestsDirectory);
        free(navigationSoDirectory);
        free(gisSoDirectory);
    }
};

TEST_F(SimulatorTest, resultsTest) {
    startSimulation(5, "astar.json", "requests.txt");
}
