#ifndef GIS_CPP_REQUESTSFILEPARSER_H
#define GIS_CPP_REQUESTSFILEPARSER_H

#include <vector>
#include "NavigationRequest.h"

class RequestsFileParser {
public:
    std::vector<NavigationRequest> parse(const std::string &filePath) const;

private:
    NavigationRequest parseLine(const std::string &line) const;
};


#endif //GIS_CPP_REQUESTSFILEPARSER_H
