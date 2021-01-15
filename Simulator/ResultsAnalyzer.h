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
    const std::string strangeGisResultsFilePath = "strange_GIS_results.log";
    std::unique_ptr<ResultsFileWriter> resultsFileWriter;

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
    std::optional<int> compareGisResultsToConsensus(int requestIndex, int navigationIndex, RequestResult *requestResult);

public:
    ResultsAnalyzer(int gisAmount, int navigationsAmount, int requestsAmount);

    void analyze();
};


#endif //GIS_CPP_RESULTSANALYZER_H
