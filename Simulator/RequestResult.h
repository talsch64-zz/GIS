#ifndef GIS_CPP_REQUESTRESULT_H
#define GIS_CPP_REQUESTRESULT_H

#include <memory>
#include "../Common/AbstractRoutes.h"

/**
 * A cell in the results table,
 * which corresponds to the score of a Navigation algorithm and request combination (across all GIS objects)
 */
class RequestResult {
    const bool consensus;
    const bool valid;
    int score = 0;
    Meters consensusShortestDistance = Meters(0);
    Minutes consensusShortestTime = Minutes(0);

public:
    /**
     * A request with consensus and valid routes for all the GIS
     * @param consensusShortestDistance
     * @param consensusShortestTime
     */
    RequestResult(const Meters &consensusShortestDistance, const Minutes &consensusShortestTime, bool valid,
                  bool consensus);

    void updateScore(int diff);

    bool isValid() const;
};


#endif //GIS_CPP_REQUESTRESULT_H
