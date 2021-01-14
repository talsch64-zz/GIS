#ifndef GIS_CPP_RESULTSANALYZER_H
#define GIS_CPP_RESULTSANALYZER_H


#include <memory>
#include "../Common/AbstractRoutes.h"
#include "RequestResult.h"

class ResultsAnalyzer {
    int gisAmount;
    int navigationsAmount;
    int requestsAmount;
    std::unique_ptr<std::unique_ptr<RequestResult>[]> resultsTable;

public:
    ResultsAnalyzer(int gisAmount, int navigationsAmount, int requestsAmount);

    void analyze();
};


#endif //GIS_CPP_RESULTSANALYZER_H
