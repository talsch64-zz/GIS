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
    std::unique_ptr<std::unique_ptr<NavigationValidator>[]> validators = std::make_unique<std::unique_ptr<NavigationValidator>[]>(
            gisAmount);
    for (int i = 0; i < gisAmount; i++) {
        validators[i] = std::make_unique<NavigationValidator>(*(sim.getGISContainer(i)->getGIS()));
    }

    for (int i = 0; i < requestsAmount; i++) {
        for (int j = 0; j < navigationsAmount; j++) {
            //a vector of results and the amount of GISs who agree on distance, time
            std::vector<std::tuple<std::unique_ptr<TaskResult>, int, int>> results;
            std::unique_ptr<TaskResult> invalidResult = nullptr, invalidDistanceResult = nullptr, invalidTimeResult = nullptr;
            for (int k = 0; k < gisAmount; k++) {
                auto &result = sim.getResult(k, j, i);
                auto &routes = result->getRoutes();

                if (result->isValid()) {
                    bool foundEqualRoutes = false;
                    for (int l = 0; l < results.size(); l++) {
                        auto &existingResultTuple = results[l];
                        auto &existingResult = std::get<0>(existingResultTuple);
                        if (routesEqual(existingResult->getRoutes().get(), routes.get())) {
                            //TODO: is it min?
                            existingResult->setGisUsageCount(std::min(existingResult->getGisUsageCount(),
                                                                      result->getGisUsageCount()));
                            foundEqualRoutes = true;
                        }
                        if (routesDistanceEqual(existingResult->getRoutes().get(), routes.get())) {
                            std::get<1>(existingResultTuple)++;
                        }
                        if (routesTimeEqual(existingResult->getRoutes().get(), routes.get())) {
                            std::get<2>(existingResultTuple)++;
                        }
                    }
                    if (!foundEqualRoutes) {
                        results.emplace_back(std::make_tuple(std::move(result), 1, 1));
                    }
                } else {
                    if (!routes->isValid() || (!result->isShortestDistanceValid() && !result->isShortestTimeValid())) {
                        invalidResult = std::move(result);
                    } else if (!result->isShortestDistanceValid()) {
                        invalidDistanceResult = std::move(result);
                    } else {
                        invalidTimeResult = std::move(result);
                    }
                }
            }

            std::unique_ptr<RequestResult> finalResult;
            if (!invalidResult && !invalidDistanceResult && !invalidTimeResult) {
                //valid result
                std::unique_ptr<TaskResult> consensusResult = nullptr;
                int agreeThreshold = gisAmount / 2;
                for (int l = 0; l < results.size() && !consensusResult; l++) {
                    auto &resultTuple = results[l];
                    if (std::get<1>(resultTuple) > agreeThreshold && std::get<2>(resultTuple) > agreeThreshold) {
                        consensusResult = std::move(std::get<0>(resultTuple));
                    }
                }
                if (consensusResult) {
                    //consensus
                    finalResult = std::make_unique<RequestResult>(
                            consensusResult->getRoutes()->shortestDistance().totalLength(),
                            consensusResult->getRoutes()->shortestTime().estimatedDuration(), false, true);
                } else {
                    //no consensus
                    finalResult = std::make_unique<RequestResult>(Meters(0), Minutes(0), true, false);
                }
            } else {
                //invalid result cell
                std::unique_ptr<TaskResult> res;
                if (invalidResult) {
                    res = std::move(invalidResult);
                } else if (invalidDistanceResult) {
                    res = std::move(invalidDistanceResult);
                } else {
                    res = std::move(invalidTimeResult);
                }
                finalResult = std::make_unique<RequestResult>(
                        res->getRoutes()->shortestDistance().totalLength(),
                        res->getRoutes()->shortestTime().estimatedDuration(), false, true);
            }
            assignResult(i, j, std::move(finalResult));
        }
    }
}

bool ResultsAnalyzer::routesEqual(AbstractRoutes *routesA, AbstractRoutes *routesB) {
    return routesDistanceEqual(routesA, routesB) && routesTimeEqual(routesA, routesB);
}

bool ResultsAnalyzer::routesDistanceEqual(AbstractRoutes *routesA, AbstractRoutes *routesB) {
    return routesA->shortestDistance().totalLength() == routesB->shortestDistance().totalLength() &&
           routesA->shortestDistance().estimatedDuration() == routesB->shortestDistance().estimatedDuration();
}

bool ResultsAnalyzer::routesTimeEqual(AbstractRoutes *routesA, AbstractRoutes *routesB) {
    return routesA->shortestTime().totalLength() == routesB->shortestTime().totalLength() &&
           routesA->shortestTime().estimatedDuration() == routesB->shortestTime().estimatedDuration();
}

void ResultsAnalyzer::assignResult(int requestIndex, int navigationIndex, std::unique_ptr<RequestResult> result) {
    int index = navigationIndex * requestsAmount + requestIndex;
    resultsTable[index] = std::move(result);
}

