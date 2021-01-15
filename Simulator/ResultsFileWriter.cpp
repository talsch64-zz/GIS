#include <fstream>
#include "ResultsFileWriter.h"

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
    const AbstractRoute &route = shortestDistance ? result.getRoutes()->shortestDistance()
                                                  : result.getRoutes()->shortestTime();
    std::string distance = std::to_string((double) route.totalLength());
    std::string time = std::to_string((double) route.estimatedDuration());
    std::string valid = result.isValid() ? "1" : "0";
    std::string sep = ", ";
    std::string msg =
            navigationName + sep + routeType + sep + gisName + sep + request.toString() + sep + distance + sep + time +
            sep + valid;
    log << msg;
    log.close();
}

void ResultsFileWriter::writeScoresTable(std::vector<std::unique_ptr<NavigationScores>> scores) {

}
