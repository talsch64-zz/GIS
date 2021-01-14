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
    const bool invalidRoutes;
    int score = 0;
    Meters consensusShortestDistance = Meters(0);
    Minutes consensusShortestTime = Minutes(0);

private:
    /**
     * A request without consensus,
     * or a request that resulted in an invalid route for at least one of the GIS
     */
    RequestResult(bool consensus);

public:
    /**
     * A request with consensus and valid routes for all the GIS
     * @param consensusShortestDistance
     * @param consensusShortestTime
     */
    RequestResult(const Meters &consensusShortestDistance, const Minutes &consensusShortestTime);

    static RequestResult createInvalidResult();

    static RequestResult createResultWithoutConsensus();

    void updateScore(int diff);
};


#endif //GIS_CPP_REQUESTRESULT_H
