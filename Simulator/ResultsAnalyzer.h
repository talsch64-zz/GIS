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

    bool routesEqual(AbstractRoutes *routesA, AbstractRoutes *routesB);

    bool routesDistanceEqual(AbstractRoutes *routesA, AbstractRoutes *routesB);

    bool routesTimeEqual(AbstractRoutes *routesA, AbstractRoutes *routesB);

    void assignResult(int requestIndex, int navigationIndex, std::unique_ptr<RequestResult> result);

    void analyzeValidTaskResult(std::unique_ptr<TaskResult> result,
                                std::vector<std::tuple<std::unique_ptr<TaskResult>, int, int>> &results);

    std::unique_ptr<RequestResult> findValidConsensusResult(std::vector<std::tuple<std::unique_ptr<TaskResult>, int, int>> &results);

public:
    ResultsAnalyzer(int gisAmount, int navigationsAmount, int requestsAmount);

    void analyze();
};


#endif //GIS_CPP_RESULTSANALYZER_H
