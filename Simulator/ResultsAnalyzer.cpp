#include "ResultsAnalyzer.h"
#include "Simulation.h"

ResultsAnalyzer::ResultsAnalyzer(int gisAmount, int navigationsAmount, int requestsAmount) : gisAmount(gisAmount),
                                                                                             navigationsAmount(
                                                                                                     navigationsAmount),
                                                                                             requestsAmount(
                                                                                                     requestsAmount) {}

void ResultsAnalyzer::analyze() {
    Simulation &sim = Simulation::getInstance();
    for (int i = 0; i < requestsAmount; i++) {
        for (int j = 0; j < navigationsAmount; j++) {
            for (int k = 0; k < gisAmount; k++) {
                auto &result = sim.getResult(k, j, i);

            }
        }
    }
}
