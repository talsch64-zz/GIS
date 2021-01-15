#include "NavigationScores.h"

void NavigationScores::addScore(int score) {
    totalScore += score;
    requestScores.push_back(score);
}

NavigationScores::NavigationScores(const std::string &navigationName) : navigationName(navigationName) {}
