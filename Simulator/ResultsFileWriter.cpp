#include <fstream>
#include "ResultsFileWriter.h"
#include "Simulation.h"

ResultsFileWriter::ResultsFileWriter(const std::string &logFilePath, const std::string &resultsFilePath) : logFilePath(
        logFilePath),
                                                                                                           resultsFilePath(
                                                                                                                   resultsFilePath) {}

void ResultsFileWriter::writeStrangeGisResult(const std::string &navigationName, const std::string &gisName,
                                              const NavigationRequest &request, const TaskResult &result,
                                              bool shortestDistance) const {
    std::ofstream log;
    log.open(logFilePath, std::ios_base::app);
    std::string routeType = "shortest_" + (shortestDistance ? std::string("distance") : std::string("time"));
    bool isValid = result.getRoutes()->isValid() && ((shortestDistance && result.isShortestDistanceValid()) ||
                                                     (!shortestDistance && result.isShortestTimeValid()));

    std::string distance = "0";
    std::string time = "0";
    if (result.getRoutes()->isValid()) {
        const AbstractRoute &route = shortestDistance ? result.getRoutes()->shortestDistance()
                                                      : result.getRoutes()->shortestTime();
        distance = std::to_string((double) route.totalLength());
        time = std::to_string((double) route.estimatedDuration());
    }

    std::string validStr = isValid ? "1" : "0";
    std::string sep = ", ";
    std::string msg =
            navigationName + sep + routeType + sep + gisName + sep + request.toString() + sep + distance + sep + time +
            sep + validStr + "\n";
    log << msg;
    log.close();
}

void ResultsFileWriter::writeScoresTable(const std::vector<std::unique_ptr<NavigationScores>> &scores,
                                         const std::vector<int> &consensusRequests) {
    Simulation &sim = Simulation::getInstance();
    std::ofstream resultsFile;
    resultsFile.open(resultsFilePath, std::ios_base::app);
    std::string sep = ", ";
    std::string titleLine = "Navigation Algorithm, ";
    for (int i: consensusRequests) {
        std::string requestStr = sim.getNavigationRequest(i).toString();
        titleLine += requestStr + sep;
    }
    titleLine += "Total Score\n";
    resultsFile << titleLine;
    for (auto &navigationScores : scores) {
        std::string navLine = navigationScores->getNavigationName() + sep;

        for (int i = 0; i < consensusRequests.size(); i++) {
            navLine += std::to_string(navigationScores->getScore(i)) + sep;
        }
        navLine += std::to_string(navigationScores->getTotalScore()) + "\n";
        resultsFile << navLine;
    }
    resultsFile.close();
}

void ResultsFileWriter::initialize() {
    //clear existing files from before this simulation
    std::ofstream logFile(logFilePath);
    std::ofstream resultsFile(resultsFilePath);
}
