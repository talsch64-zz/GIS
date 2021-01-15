#ifndef GIS_CPP_REQUESTRESULT_H
#define GIS_CPP_REQUESTRESULT_H

#include <memory>
#include "../Common/AbstractRoutes.h"

/**
 * A cell in the results table,
 * which corresponds to the score of a Navigation algorithm and request combination (across all GIS objects)
 */
class RequestResult {
    const bool valid;
    int score = 0;
    std::pair<Meters, Minutes> consensusShortestDistance;
    std::pair<Meters, Minutes> consensusShortestTime;
    int gisRequests;

public:
    /**
     * An invalid request
     */
    RequestResult();

    /**
     * A request with consensus and valid routes for all the GIS
     * @param consensusShortestDistance
     * @param consensusShortestTime
     */
    RequestResult(std::pair<Meters, Minutes> consensusShortestDistance,
                  std::pair<Meters, Minutes> consensusShortestTime);

    void updateScore(int diff);

    bool isValid() const;

    int getScore() const;

    const std::pair<Meters, Minutes> &getConsensusShortestDistance() const;

    const std::pair<Meters, Minutes> &getConsensusShortestTime() const;

    int getGisRequests() const;

    void setGisRequests(int gisRequests);
};


#endif //GIS_CPP_REQUESTRESULT_H
