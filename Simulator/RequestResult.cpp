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

int RequestResult::getScore() const {
    return score;
}

const std::pair<Meters, Minutes> &RequestResult::getConsensusShortestDistance() const {
    return consensusShortestDistance;
}

const std::pair<Meters, Minutes> &RequestResult::getConsensusShortestTime() const {
    return consensusShortestTime;
}

int RequestResult::getGisDistanceRequests() const {
    return gisDistanceRequests;
}

void RequestResult::setGisDistanceRequests(int gisRequests) {
    RequestResult::gisDistanceRequests = gisRequests;
}

int RequestResult::getGisTimeRequests() const {
    return gisTimeRequests;
}

void RequestResult::setGisTimeRequests(int gisRequests) {
    RequestResult::gisTimeRequests = gisRequests;
}
