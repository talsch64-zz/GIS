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
        NavigationRequest req = sim.getNavigationRequest(i);
        for (int j = 0; j < navigationsAmount; j++) {
            std::optional<Meters> shortestDistance;
            std::optional<Minutes> shortestTime;
            int agreeOnDistance = 0, agreeOnTime = 0;
            bool foundInvalid = false, foundInvalidDistance = false, foundInvalidTime = false;
            for (int k = 0; k < gisAmount; k++) {
                std::unique_ptr<NavigationValidator> &validator = validators[k];
                auto &result = sim.getResult(k, j, i);
//                bool isValid = true;
                //Check if result is valid
//                if (result->isValid()) {
//                    if (!validator->validateRoute(req.getFrom(), req.getTo(), result->shortestDistance())) {
//                        isValid = false;
//                        foundInvalidDistance = true;
//                    }
//                    if (!validator->validateRoute(req.getFrom(), req.getTo(), result->shortestTime())) {
//                        isValid = false;
//                        foundInvalidTime = true;
//                    }
//                } else {
//                    isValid = false;
//                }
                auto &routes = result->getRoutes();
                bool isValid = routes->isValid() && result->isShortestDistanceValid() && result->isShortestTimeValid();

                if (isValid) {
                    if (!shortestDistance.has_value() ||
                        routes->shortestDistance().totalLength() < shortestDistance.value()) {
                        shortestDistance = routes->shortestDistance().totalLength();
                        agreeOnDistance = 1;
                    } else if (routes->shortestDistance().totalLength() == shortestDistance.value()) {
                        agreeOnDistance++;
                    }
                } else {
                    foundInvalid = true;
                }
            }
        }
    }
}
