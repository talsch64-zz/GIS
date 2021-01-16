#ifndef GIS_CPP_RESULTSANALYZER_H
#define GIS_CPP_RESULTSANALYZER_H


#include <memory>
#include "../Common/AbstractRoutes.h"
#include "RequestResult.h"
#include "TaskResult.h"
#include "ResultsFileWriter.h"

class ResultsAnalyzer {
    int gisAmount;
    int navigationsAmount;
    int requestsAmount;
    std::unique_ptr<std::unique_ptr<RequestResult>[]> resultsTable;
    //vector of request indices that achieved consensus for all algorithms
    std::vector<int> consensusRequests;
    const std::string strangeGisResultsFilePath = "strange_GIS_results.log";
    const std::string resultsFilePath = "simulation.results";
    std::unique_ptr<ResultsFileWriter> resultsFileWriter;

public:
    ResultsAnalyzer(int gisAmount, int navigationsAmount, int requestsAmount, const std::string &outputDirectory);

    void analyze();

    void writeResultsToFile();

private:
    std::unique_ptr<RequestResult> &getResult(int requestIndex, int navigationIndex);

    void assignResult(int requestIndex, int navigationIndex, std::unique_ptr<RequestResult> result);

    void analyzeValidTaskResult(std::pair<Meters, Minutes> result,
                                std::vector<std::pair<std::pair<Meters, Minutes>, int>> &results);

    std::optional<std::pair<Meters, Minutes>>
    findValidConsensusResult(std::vector<std::pair<std::pair<Meters, Minutes>, int>> &results);

    /**
     * Write strange Gis results to log
     * @param requestIndex
     * @param navigationIndex
     * @param requestResult
     * @return minimal Gis requests, out of Gis which got best distance/time (if a consensus was reached)
     */
    std::optional<int>
    compareGisResultsToConsensus(int requestIndex, int navigationIndex, RequestResult *requestResult);

    /**
     * Compare shortest distance routes
     * @param routeA
     * @param routeB
     * @return 1 if routeA is better, 0 if equal, -1 if routeB is better
     */
    int compareDistanceRoutes(std::pair<Meters, Minutes> routeA, std::pair<Meters, Minutes> routeB);

    /**
     * Compare shortest time routes
     * @param routeA
     * @param routeB
     * @return 1 if routeA is better, 0 if equal, -1 if routeB is better
     */
    int compareTimeRoutes(std::pair<Meters, Minutes> routeA, std::pair<Meters, Minutes> routeB);

    /**
     * Update the score of navigation algorithms which found minimal distance/time route.
     * this function is only called if all navigation algorithms reached a consensus for the request.
     * @param requestIndex
     */
    void updateBestRouteScores(int requestIndex, std::pair<Meters, Minutes> bestDistanceRoute,
                               std::pair<Meters, Minutes> bestTimeRoute);
};


#endif //GIS_CPP_RESULTSANALYZER_H
