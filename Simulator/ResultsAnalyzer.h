#ifndef GIS_CPP_RESULTSANALYZER_H
#define GIS_CPP_RESULTSANALYZER_H


#include <memory>
#include "../Common/AbstractRoutes.h"
#include "RequestResult.h"
#include "TaskResult.h"

class ResultsAnalyzer {
    int gisAmount;
    int navigationsAmount;
    int requestsAmount;
    std::unique_ptr<std::unique_ptr<RequestResult>[]> resultsTable;

    void assignResult(int requestIndex, int navigationIndex, std::unique_ptr<RequestResult> result);

    void analyzeValidTaskResult(std::pair<Meters, Minutes> result,
                                std::vector<std::pair<std::pair<Meters, Minutes>, int>> &results);

    std::optional<std::pair<Meters, Minutes>>
    findValidConsensusResult(std::vector<std::pair<std::pair<Meters, Minutes>, int>> &results);

public:
    ResultsAnalyzer(int gisAmount, int navigationsAmount, int requestsAmount);

    void analyze();
};


#endif //GIS_CPP_RESULTSANALYZER_H
