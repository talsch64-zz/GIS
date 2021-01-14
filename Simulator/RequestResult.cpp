#include "RequestResult.h"

RequestResult::RequestResult(const Meters &consensusShortestDistance, const Minutes &consensusShortestTime, bool valid,
                             bool consensus)
        : valid(valid), consensus(consensus), consensusShortestDistance(consensusShortestDistance),
          consensusShortestTime(consensusShortestTime) {}

void RequestResult::updateScore(int diff) {
    score += diff;
}

bool RequestResult::isValid() const {
    return valid;
}
