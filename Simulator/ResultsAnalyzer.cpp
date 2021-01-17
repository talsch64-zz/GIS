#include <random>
#include "ResultsAnalyzer.h"
#include "Simulation.h"
#include "NavigationValidator.h"

ResultsAnalyzer::ResultsAnalyzer(int gisAmount, int navigationsAmount, int requestsAmount,
                                 const std::string &outputDirectory) : gisAmount(gisAmount),
                                                                       navigationsAmount(
                                                                               navigationsAmount),
                                                                       requestsAmount(
                                                                               requestsAmount),
                                                                       resultsTable(
                                                                               std::make_unique<std::unique_ptr<RequestResult>[]>(
                                                                                       navigationsAmount *
                                                                                       requestsAmount)),
                                                                       resultsFileWriter(
                                                                               std::make_unique<ResultsFileWriter>(
                                                                                       outputDirectory + "/" +
                                                                                       strangeGisResultsFilePath,
                                                                                       outputDirectory + "/" +
                                                                                       resultsFilePath)) {}

void ResultsAnalyzer::analyze() {
    resultsFileWriter->initialize();
    Simulation &sim = Simulation::getInstance();

    for (int i = 0; i < requestsAmount; i++) {
        //do all navigation algorithms reach consensus for this request
        bool columnConsensus = true;
        //best distance and time routes between all navigation algorithms for this request
        std::optional<std::pair<Meters, Minutes>> bestDistanceRoute;
        std::optional<std::pair<Meters, Minutes>> bestTimeRoute;

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
                    if (!bestDistanceRoute.has_value() ||
                        compareDistanceRoutes(distanceConsensus.value(), bestDistanceRoute.value()) == 1) {
                        bestDistanceRoute = distanceConsensus;
                    }
                    if (!bestTimeRoute.has_value() ||
                        compareTimeRoutes(timeConsensus.value(), bestTimeRoute.value()) == 1) {
                        bestTimeRoute = timeConsensus;
                    }
                } else {
                    //no consensus
                    finalResult = nullptr;
                    columnConsensus = false;
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
            if (minGisRequests.first.has_value()) {
                finalResult->setGisDistanceRequests(minGisRequests.first.value());
            }
            if (minGisRequests.second.has_value()) {
                finalResult->setGisTimeRequests(minGisRequests.second.value());
            }
            assignResult(i, j, std::move(finalResult));
        }
        if (columnConsensus) {
            consensusRequests.push_back(i);
            if (bestDistanceRoute.has_value() && bestTimeRoute.has_value()) {
                //some navigation algorithms returned valid routes
                updateBestRouteScores(i, bestDistanceRoute.value(), bestTimeRoute.value());
            }
        }
    }
}

std::unique_ptr<RequestResult> &ResultsAnalyzer::getResult(int requestIndex, int navigationIndex) {
    int index = navigationIndex * requestsAmount + requestIndex;
    return resultsTable[index];
}

void ResultsAnalyzer::assignResult(int requestIndex, int navigationIndex, std::unique_ptr<RequestResult> result) {
    int index = navigationIndex * requestsAmount + requestIndex;
    resultsTable[index] = std::move(result);
}

void ResultsAnalyzer::analyzeValidTaskResult(std::pair<Meters, Minutes> result,
                                             std::vector<std::pair<std::pair<Meters, Minutes>, int>> &results) {
    bool foundEqualRoute = false;
    for (auto &existingResultPair : results) {
        auto &existingResult = existingResultPair.first;
        if (existingResult.first == result.first &&
            existingResult.second == result.second) {
            //increase count of GIS who agree on result
            existingResultPair.second++;
            foundEqualRoute = true;
            break;
        }
    }
    if (!foundEqualRoute) {
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

std::pair<std::optional<int>, std::optional<int>>
ResultsAnalyzer::compareGisResultsToConsensus(int requestIndex, int navigationIndex, RequestResult *requestResult) {
    Simulation &sim = Simulation::getInstance();
    NavigationRequest navigationRequest = sim.getNavigationRequest(navigationIndex);

    std::string navigationName = sim.getNavigationContainer(navigationIndex)->getName();
    std::pair<std::optional<int>, std::optional<int>> minGisRequests;
    for (int k = 0; k < gisAmount; k++) {
        std::string gisName = sim.getGISContainer(k)->getName();
        auto &taskResult = sim.getResult(k, navigationIndex, requestIndex);
        auto &routes = taskResult->getRoutes();

        //null request result indicates a consensus wasn't reached
        if (requestResult != nullptr && requestResult->isValid()) {
            //valid and consensus
            if (routes->shortestDistance().totalLength() != requestResult->getConsensusShortestDistance().first ||
                routes->shortestDistance().estimatedDuration() !=
                requestResult->getConsensusShortestDistance().second) {
                //GIS doesn't agree with consensus on shortest distance
                resultsFileWriter->writeStrangeGisResult(navigationName, gisName, navigationRequest, *taskResult,
                                                         true);
            } else if (!minGisRequests.first.has_value() ||
                       taskResult->getGisUsageCount() < minGisRequests.first.value()) {
                //GIS agrees with consensus and has minimal requests
                //each GIS which agrees with consensus on either shortest distance or time is eligible to affect minimal GIS requests
                minGisRequests.first = taskResult->getGisUsageCount();
            }

            if (routes->shortestTime().totalLength() != requestResult->getConsensusShortestTime().first ||
                routes->shortestTime().estimatedDuration() !=
                requestResult->getConsensusShortestTime().second) {
                //GIS doesn't agree with consensus on shortest time
                resultsFileWriter->writeStrangeGisResult(navigationName, gisName, navigationRequest, *taskResult,
                                                         false);
            } else if (!minGisRequests.second.has_value() ||
                       taskResult->getGisUsageCount() < minGisRequests.second.value()) {
                //GIS agrees with consensus and has minimal requests
                //each GIS which agrees with consensus on either shortest distance or time is eligible to affect minimal GIS requests
                minGisRequests.second = taskResult->getGisUsageCount();
            }
        } else {
            //invalid or no consensus - all results should be written to log
            resultsFileWriter->writeStrangeGisResult(navigationName, gisName, navigationRequest, *taskResult,
                                                     true);
            resultsFileWriter->writeStrangeGisResult(navigationName, gisName, navigationRequest, *taskResult,
                                                     false);
        }
    }

    return minGisRequests;
}

int ResultsAnalyzer::compareDistanceRoutes(std::pair<Meters, Minutes> routeA, std::pair<Meters, Minutes> routeB) {
    int ret = -1;
    if (routeA.first == routeB.first && routeA.second == routeB.second) {
        ret = 0;
    } else if (routeA.first < routeB.first ||
               (routeA.first == routeB.first && routeA.second < routeB.second)) {
        ret = 1;
    }
    return ret;
}

int ResultsAnalyzer::compareTimeRoutes(std::pair<Meters, Minutes> routeA, std::pair<Meters, Minutes> routeB) {
    int ret = -1;
    if (routeA.first == routeB.first && routeA.second == routeB.second) {
        ret = 0;
    } else if (routeA.second < routeB.second ||
               (routeA.second == routeB.second && routeA.first < routeB.first)) {
        ret = 1;
    }
    return ret;
}

void ResultsAnalyzer::updateBestRouteScores(int requestIndex, std::pair<Meters, Minutes> bestDistanceRoute,
                                            std::pair<Meters, Minutes> bestTimeRoute) {
    //find minimal Gis requests out of algorithms which found best distance/time route
    //update score of algorithms which found the best distance/time route
    std::pair<std::optional<int>, std::optional<int>> minGisRequests;
    for (int j = 0; j < navigationsAmount; j++) {
        auto &requestResult = getResult(requestIndex, j);
        if (requestResult->isValid()) {
            bool bestRoute = false;
            if (compareDistanceRoutes(requestResult->getConsensusShortestDistance(), bestDistanceRoute) == 0) {
                requestResult->updateScore(1);
                bestRoute = true;
            }
            if (compareTimeRoutes(requestResult->getConsensusShortestTime(), bestTimeRoute) == 0) {
                requestResult->updateScore(1);
                bestRoute = true;
            }
            if (bestRoute && (!minGisRequests.first.has_value() ||
                              requestResult->getGisDistanceRequests() < minGisRequests.first.value())) {
                minGisRequests.first = requestResult->getGisDistanceRequests();
            }
            if (bestRoute && (!minGisRequests.second.has_value() ||
                              requestResult->getGisTimeRequests() < minGisRequests.second.value())) {
                minGisRequests.second = requestResult->getGisTimeRequests();
            }
        }
    }

    //update score of algorithms which found the best distance/time route and have minimal Gis requests
    if (minGisRequests.first.has_value() || minGisRequests.second.has_value()) {
        for (int j = 0; j < navigationsAmount; j++) {
            auto &requestResult = getResult(requestIndex, j);
            if (requestResult->isValid()) {
                if (requestResult->getGisDistanceRequests() == minGisRequests.first.value() &&
                    compareDistanceRoutes(requestResult->getConsensusShortestDistance(), bestDistanceRoute) == 0) {
                    requestResult->updateScore(1);
                }
                if (requestResult->getGisTimeRequests() == minGisRequests.second.value() &&
                    compareTimeRoutes(requestResult->getConsensusShortestTime(), bestTimeRoute) == 0) {
                    requestResult->updateScore(1);
                }
            }
        }
    }
}

void ResultsAnalyzer::writeResultsToFile() {
    Simulation &sim = Simulation::getInstance();
    std::vector<std::unique_ptr<NavigationScores>> scores;
    for (int i = 0; i < navigationsAmount; i++) {
        std::string name = sim.getNavigationContainer(i)->getName();
        scores.emplace_back(std::make_unique<NavigationScores>(name));
    }

    //TODO: remove random
//    std::default_random_engine generator;
//    std::uniform_int_distribution<int> distribution(1,100);
    for (int i : consensusRequests) {
        for (int j = 0; j < navigationsAmount; j++) {
            auto &result = getResult(i, j);
            scores[j]->addScore(result->getScore());
        }
    }

    //sort by total score
    std::sort(scores.begin(), scores.end(),
              [](std::unique_ptr<NavigationScores> &navScoreA, std::unique_ptr<NavigationScores> &navScoreB) {
                  return navScoreA->getTotalScore() > navScoreB->getTotalScore();
              });

    resultsFileWriter->writeScoresTable(std::move(scores), std::move(consensusRequests));
}
