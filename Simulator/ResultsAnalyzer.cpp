#include "ResultsAnalyzer.h"
#include "Simulation.h"
#include "NavigationValidator.h"

ResultsAnalyzer::ResultsAnalyzer(int gisAmount, int navigationsAmount, int requestsAmount) : gisAmount(gisAmount),
                                                                                             navigationsAmount(
                                                                                                     navigationsAmount),
                                                                                             requestsAmount(
                                                                                                     requestsAmount) {}

void ResultsAnalyzer::analyze() {
    Simulation &sim = Simulation::getInstance();

    for (int i = 0; i < requestsAmount; i++) {
        for (int j = 0; j < navigationsAmount; j++) {
            //a vector of Meters, Minutes result of Route, and the amount of GISs who agree on them (one vector for distance, one for time)
            auto distanceResults = std::make_unique<std::vector<std::pair<std::pair<Meters, Minutes>, int>>>();
            auto timeResults = std::make_unique<std::vector<std::pair<std::pair<Meters, Minutes>, int>>>();
            bool invalidResult = false, invalidDistanceResult = false, invalidTimeResult = false;
            for (int k = 0; k < gisAmount; k++) {
                auto &result = sim.getResult(k, j, i);
                auto &routes = result->getRoutes();

                if (result->isValid()) {
                    //valid route
                    //analyze for shortest distance
                    analyzeValidTaskResult(std::make_pair(routes->shortestDistance().totalLength(),
                                                          routes->shortestDistance().estimatedDuration()),
                                           *distanceResults);
                    //analyze for shortest time
                    analyzeValidTaskResult(std::make_pair(routes->shortestTime().totalLength(),
                                                          routes->shortestTime().estimatedDuration()),
                                           *timeResults);
                } else {
                    //invalid route
                    if (!result->getRoutes()->isValid()) {
                        //no routes were found
                        invalidResult = true;
                    }
                    if (!result->isShortestDistanceValid()) {
                        //shortest distance route is invalid
                        invalidDistanceResult = true;
                    }
                    if (!result->isShortestTimeValid()) {
                        //shortest time route is invalid
                        invalidTimeResult = true;
                    }
                }

                std::unique_ptr<RequestResult> finalResult;
                if (!invalidResult && !invalidDistanceResult && !invalidTimeResult) {
                    //valid result
                    auto distanceConsensus = findValidConsensusResult(*distanceResults);
                    auto timeConsensus = findValidConsensusResult(*timeResults);
                    if (distanceConsensus.has_value() && timeConsensus.has_value()) {
                        //has consensus
                        finalResult = std::make_unique<RequestResult>(distanceConsensus.value(), timeConsensus.value());
                    } else {
                        //no consensus
                        finalResult = nullptr;
                    }
                } else {
                    //invalid result cell
                    finalResult = std::make_unique<RequestResult>();
                }
                assignResult(i, j, std::move(finalResult));
            }
        }
    }
}

void ResultsAnalyzer::assignResult(int requestIndex, int navigationIndex, std::unique_ptr<RequestResult> result) {
    int index = navigationIndex * requestsAmount + requestIndex;
    resultsTable[index] = std::move(result);
}

void ResultsAnalyzer::analyzeValidTaskResult(std::pair<Meters, Minutes> result,
                                             std::vector<std::pair<std::pair<Meters, Minutes>, int>> &results) {
    bool foundEqualDistanceRoute = false;
    for (auto &existingResultPair : results) {
        auto &existingResult = existingResultPair.first;
        if (existingResult.first == result.first &&
            existingResult.second == result.second) {
            //increase count of GIS who agree on result
            existingResultPair.second++;
            foundEqualDistanceRoute = true;
        }
    }
    if (!foundEqualDistanceRoute) {
        //the result is a route different from all existing ones
        results.emplace_back(result, 1);
    }
}

std::optional<std::pair<Meters, Minutes>>
ResultsAnalyzer::findValidConsensusResult(std::vector<std::pair<std::pair<Meters, Minutes>, int>> &results) {
    std::optional<std::pair<Meters, Minutes>> consensusResult;
    int agreeThreshold = gisAmount / 2;
    for (int i = 0; i < results.size() && !consensusResult.has_value(); i++) {
        auto &resultPair = results[i];
        if (resultPair.second > agreeThreshold) {
            consensusResult = resultPair.first;
        }
    }

    return consensusResult;
}
