#include "ResultsAnalyzer.h"
#include "Simulation.h"
#include "NavigationValidator.h"

ResultsAnalyzer::ResultsAnalyzer(int gisAmount, int navigationsAmount, int requestsAmount) : gisAmount(gisAmount),
                                                                                             navigationsAmount(
                                                                                                     navigationsAmount),
                                                                                             requestsAmount(
                                                                                                     requestsAmount),
                                                                                             resultsFileWriter(
                                                                                                     std::make_unique<ResultsFileWriter>(
                                                                                                             strangeGisResultsFilePath)) {}

void ResultsAnalyzer::analyze() {
    Simulation &sim = Simulation::getInstance();

    for (int i = 0; i < requestsAmount; i++) {
        for (int j = 0; j < navigationsAmount; j++) {
            //a vector of Meters, Minutes result of Route, and the amount of GISs who agree on them (one vector for distance, one for time)
            auto distanceResults = std::make_unique<std::vector<std::pair<std::pair<Meters, Minutes>, int>>>();
            auto timeResults = std::make_unique<std::vector<std::pair<std::pair<Meters, Minutes>, int>>>();
            bool invalidDistanceResult = false, invalidTimeResult = false;
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
                    if (!result->getRoutes()->isValid() || !result->isShortestDistanceValid()) {
                        invalidDistanceResult = true;
                    }
                    if (!result->getRoutes()->isValid() || !result->isShortestTimeValid()) {
                        invalidTimeResult = true;
                    }
                }
            }

            std::unique_ptr<RequestResult> finalResult;
            if (!invalidDistanceResult && !invalidTimeResult) {
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
                if (invalidDistanceResult && invalidTimeResult) {
                    finalResult->updateScore(-2);
                } else {
                    finalResult->updateScore(-1);
                }
            }
            auto minGisRequests = compareGisResultsToConsensus(i, j, finalResult.get());
            if (minGisRequests.has_value()) {
                finalResult->setGisRequests(minGisRequests.value());
            }
            assignResult(i, j, std::move(finalResult));
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

std::optional<int>
ResultsAnalyzer::compareGisResultsToConsensus(int requestIndex, int navigationIndex, RequestResult *requestResult) {
    Simulation &sim = Simulation::getInstance();
    NavigationRequest navigationRequest = sim.getNavigationRequest(navigationIndex);

    std::string navigationName = sim.getNavigationContainer(navigationIndex)->getName();
    std::optional<int> minGisRequests;
    for (int k = 0; k < gisAmount; k++) {
        std::string gisName = sim.getGISContainer(k)->getName();
        auto &result = sim.getResult(k, navigationIndex, requestIndex);
        auto &routes = result->getRoutes();

        if (requestResult != nullptr && requestResult->isValid()) {
            //valid and consensus
            if (routes->shortestDistance().totalLength() != requestResult->getConsensusShortestDistance().first ||
                routes->shortestDistance().estimatedDuration() !=
                requestResult->getConsensusShortestDistance().second) {
                //GIS doesn't agree with consensus on shortest distance
                resultsFileWriter->writeStrangeGisResult(navigationName, gisName, navigationRequest, *result,
                                                         true);
            } else if (!minGisRequests.has_value() || result->getGisUsageCount() < minGisRequests.value()) {
                //GIS agrees with consensus and has minimal requests
                minGisRequests = result->getGisUsageCount();
            }

            if (routes->shortestTime().totalLength() != requestResult->getConsensusShortestTime().first ||
                routes->shortestTime().estimatedDuration() !=
                requestResult->getConsensusShortestTime().second) {
                //GIS doesn't agree with consensus on shortest time
                resultsFileWriter->writeStrangeGisResult(navigationName, gisName, navigationRequest, *result,
                                                         false);
            } else if (!minGisRequests.has_value() || result->getGisUsageCount() < minGisRequests.value()) {
                //GIS agrees with consensus and has minimal requests
                minGisRequests = result->getGisUsageCount();
            }
        } else {
            //invalid or no consensus - all results should be written to log
            resultsFileWriter->writeStrangeGisResult(navigationName, gisName, navigationRequest, *result,
                                                     true);
            resultsFileWriter->writeStrangeGisResult(navigationName, gisName, navigationRequest, *result,
                                                     false);
        }
    }

    return minGisRequests;
}
