#ifndef GIS_CPP_RESULTSFILEWRITER_H
#define GIS_CPP_RESULTSFILEWRITER_H


#include "TaskResult.h"
#include "NavigationRequest.h"
#include "NavigationScores.h"

class ResultsFileWriter {
    const std::string logFilePath;
    const std::string resultsFilePath;

public:
    ResultsFileWriter(const std::string &logFilePath, const std::string &resultsFilePath);

    void initialize();

    void writeStrangeGisResult(const std::string &navigationName, const std::string &gisName,
                               const NavigationRequest &request, const TaskResult &result, bool shortestDistance) const;

    void writeScoresTable(const std::vector<std::unique_ptr<NavigationScores>> &scores,
                          const std::vector<int> &consensusRequests);
};


#endif //GIS_CPP_RESULTSFILEWRITER_H
