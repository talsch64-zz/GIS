#include "gtest/gtest.h"
#include "../../Simulator/Simulation.h"
#include "../mocks/NavigationTasksManagerMock.h"
#include "../../Navigation/Routes.h"
#include <fstream>
#include <cstring>
#include <string>
#include <regex>


class SimulatorTest : public ::testing::Test {
    char *mapsDirectory = strdup("/home/student/repos/advanced-topics/advanced-ex1/tests/");
    char *requestsDirectory = strdup("/home/student/repos/advanced-topics/advanced-ex1/tests/Simulator/");
    char *navigationSoDirectory = strdup("/home/student/repos/advanced-topics/advanced-ex1/Navigation/old-obuntu-so");
    char *gisSoDirectory = strdup("/home/student/repos/advanced-topics/advanced-ex1/GIS/old-obuntu-so");

public:
    std::unique_ptr<RequestsFileParser> requestsFileParser;

    SimulatorTest() : requestsFileParser(std::make_unique<RequestsFileParser>()) {
    }

    void SetUp() override {
    }

    void prepareSimulation(int numThreads, std::string mapFileName, std::string requestsFileName) {
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

    std::vector<std::string> getResultsLines() {
        std::vector<std::string> lines;
        std::ifstream infile("simulation.results");
        std::string line;
        while (std::getline(infile, line)) {
            lines.push_back(line);
        }
        lines.pop_back();
        return lines;
    }

    void assertResults(std::vector<std::string> expectedResults, std::vector<NavigationRequest> requests) {
        auto resultLines = getResultsLines();
        std::string title = resultLines[0];
        std::string expectedTitle = "Navigation Algorithm";
        for (auto req: requests) {
            expectedTitle += ", " + req.toString();
        }
        expectedTitle += ", Total Score";
        EXPECT_EQ(title, expectedTitle);

        for (int i = 1; i < resultLines.size(); i++) {
            EXPECT_EQ(resultLines[i], expectedResults[i - 1]);
        }
    }

    virtual ~SimulatorTest() {
        free(mapsDirectory);
        free(requestsDirectory);
        free(navigationSoDirectory);
        free(gisSoDirectory);
    }
};

TEST_F(SimulatorTest, resultsTest) {
    prepareSimulation(5, "astar.json", "requests.txt");
    Simulation &simulation = Simulation::getInstance();
    auto &registrar = simulation.getRegistrar();
    auto requests = requestsFileParser->parse(registrar->getNavigationRequestsPath());
    std::unique_ptr<NavigationTasksManagerMock> taskManager = std::make_unique<NavigationTasksManagerMock>(3, 3, 2);
    std::vector<std::pair<EntityId, Direction>> mockWays;

    auto routes000 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1107.43), Minutes(71.5),
                                    mockWays),
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1107.43), Minutes(61.2),
                                    mockWays), true, "");
    taskManager->setResult(0, 0, 0,
                           std::make_unique<TaskResult>(std::move(routes000), true, true, 25));

    auto routes100 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(503.5), Minutes(71.5),
                                    mockWays),
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(106.2), Minutes(44),
                                    mockWays), true, "");
    taskManager->setResult(1, 0, 0,
                           std::make_unique<TaskResult>(std::move(routes100), true, true, 27));

    auto routes200 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1107.43), Minutes(71.5),
                                    mockWays),
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1107.43), Minutes(61.2),
                                    mockWays), true, "");
    taskManager->setResult(2, 0, 0,
                           std::make_unique<TaskResult>(std::move(routes200), true, true, 16));

    auto routes010 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1006.8), Minutes(71.5),
                                    mockWays),
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1200.93), Minutes(60),
                                    mockWays), true, "");
    taskManager->setResult(0, 1, 0,
                           std::make_unique<TaskResult>(std::move(routes010), true, true, 101));

    auto routes110 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1006.8), Minutes(71.5),
                                    mockWays),
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1125), Minutes(69.65),
                                    mockWays), true, "");
    taskManager->setResult(1, 1, 0,
                           std::make_unique<TaskResult>(std::move(routes110), true, true, 100));

    auto routes210 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1345.12), Minutes(89.12),
                                    mockWays),
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1200.93), Minutes(60),
                                    mockWays), true, "");
    taskManager->setResult(2, 1, 0,
                           std::make_unique<TaskResult>(std::move(routes210), true, true, 95));

    auto routes020 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1276.8), Minutes(73.5),
                                    mockWays),
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1200.93), Minutes(60),
                                    mockWays), true, "");
    taskManager->setResult(0, 2, 0,
                           std::make_unique<TaskResult>(std::move(routes020), true, true, 99));

    auto routes120 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1076.8), Minutes(71.5),
                                    mockWays),
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1200.93), Minutes(60),
                                    mockWays), true, "");
    taskManager->setResult(1, 2, 0,
                           std::make_unique<TaskResult>(std::move(routes120), true, true, 94));

    auto routes220 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1076.8), Minutes(71.5),
                                    mockWays),
            std::make_unique<Route>(requests[0].getFrom(), requests[0].getTo(), Meters(1203.13), Minutes(60),
                                    mockWays), true, "");
    taskManager->setResult(2, 2, 0,
                           std::make_unique<TaskResult>(std::move(routes220), true, true, 50));

    auto routes001 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(32745), Minutes(202.15),
                                    mockWays),
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(32745), Minutes(202.15),
                                    mockWays), true, "");
    taskManager->setResult(0, 0, 1,
                           std::make_unique<TaskResult>(std::move(routes001), true, true, 78));

    auto routes101 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(12), Minutes(59),
                                    mockWays),
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(134), Minutes(20),
                                    mockWays), true, "");
    taskManager->setResult(1, 0, 1,
                           std::make_unique<TaskResult>(std::move(routes101), false, true, 555));

    auto routes201 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(32745), Minutes(202.15),
                                    mockWays),
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(32745), Minutes(202.15),
                                    mockWays), true, "");
    taskManager->setResult(2, 0, 1,
                           std::make_unique<TaskResult>(std::move(routes201), true, true, 108));

    auto routes011 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(40523.1), Minutes(208.3),
                                    mockWays),
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(40523.1), Minutes(208.3),
                                    mockWays), true, "");
    taskManager->setResult(0, 1, 1,
                           std::make_unique<TaskResult>(std::move(routes011), true, true, 301));

    auto routes111 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(40523.1), Minutes(208.3),
                                    mockWays),
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(40523.1), Minutes(208.3),
                                    mockWays), true, "");
    taskManager->setResult(1, 1, 1,
                           std::make_unique<TaskResult>(std::move(routes111), true, true, 301));

    auto routes211 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(19803.3), Minutes(208.3),
                                    mockWays),
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(40523.1), Minutes(208.3),
                                    mockWays), true, "");
    taskManager->setResult(2, 1, 1,
                           std::make_unique<TaskResult>(std::move(routes211), true, true, 321));

    auto routes021 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(40523.1), Minutes(208.4),
                                    mockWays),
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(40522.8), Minutes(208.3),
                                    mockWays), true, "");
    taskManager->setResult(0, 2, 1,
                           std::make_unique<TaskResult>(std::move(routes021), true, true, 120));

    auto routes121 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(40523.1), Minutes(208.4),
                                    mockWays),
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(49525.2), Minutes(259.11),
                                    mockWays), true, "");
    taskManager->setResult(1, 2, 1,
                           std::make_unique<TaskResult>(std::move(routes121), true, true, 111));

    auto routes221 = std::make_unique<Routes>(
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(40523.1), Minutes(208.4),
                                    mockWays),
            std::make_unique<Route>(requests[1].getFrom(), requests[1].getTo(), Meters(40522.8), Minutes(208.3),
                                    mockWays), true, "");
    taskManager->setResult(2, 2, 1,
                           std::make_unique<TaskResult>(std::move(routes221), true, true, 908));

    simulation.setTaskManager(std::move(taskManager));

    std::vector<std::string> expectedResults;
    expectedResults.push_back(simulation.getNavigationContainer(1)->getName() + ", 3, 2, 5");
    expectedResults.push_back(simulation.getNavigationContainer(2)->getName() + ", 2, 2, 4");
    expectedResults.push_back(simulation.getNavigationContainer(0)->getName() + ", 0, -1, -1");

    simulation.startSimulation();

    assertResults(expectedResults, requests);
}
