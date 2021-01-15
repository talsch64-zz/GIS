#ifndef GIS_CPP_RESULTSFILEWRITER_H
#define GIS_CPP_RESULTSFILEWRITER_H


#include "TaskResult.h"
#include "NavigationRequest.h"

class ResultsFileWriter {
    const std::string filePath;

public:
    ResultsFileWriter(const std::string &filePath);

    void writeStrangeGisResult(const std::string &navigationName, const std::string &gisName,
                               const NavigationRequest &request, const TaskResult &result, bool shortestDistance) const;
};


#endif //GIS_CPP_RESULTSFILEWRITER_H
