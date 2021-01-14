#include "RequestResult.h"

RequestResult::RequestResult(bool consensus) : consensus(consensus), invalidRoutes(!consensus) {
}

RequestResult::RequestResult(const Meters &consensusShortestDistance, const Minutes &consensusShortestTime)
        : invalidRoutes(false), consensus(true), consensusShortestDistance(consensusShortestDistance),
          consensusShortestTime(consensusShortestTime) {}

void RequestResult::updateScore(int diff) {
    score += diff;
}

RequestResult RequestResult::createInvalidResult() {
    return RequestResult(true);
}

RequestResult RequestResult::createResultWithoutConsensus() {
    return RequestResult(false);
}
