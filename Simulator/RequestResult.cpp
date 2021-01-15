#include "RequestResult.h"

RequestResult::RequestResult() : valid(false), consensusShortestDistance(std::make_pair(Meters(0), Minutes(0))),
                                 consensusShortestTime(std::make_pair(Meters(0), Minutes(0))) {
}


RequestResult::RequestResult(std::pair<Meters, Minutes> consensusShortestDistance,
                             std::pair<Meters, Minutes> consensusShortestTime)
        : valid(true), consensusShortestDistance(std::move(consensusShortestDistance)),
          consensusShortestTime(std::move(consensusShortestTime)) {}

void RequestResult::updateScore(int diff) {
    score += diff;
}

bool RequestResult::isValid() const {
    return valid;
}