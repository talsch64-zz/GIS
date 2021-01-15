#include "NavigationScores.h"

void NavigationScores::addScore(int score) {
    totalScore += score;
    requestScores.push_back(score);
}

NavigationScores::NavigationScores(const std::string &navigationName) : navigationName(navigationName) {}

int NavigationScores::getScore(int index) const {
    return requestScores[index];
}

int NavigationScores::getTotalScore() const {
    return totalScore;
}

const std::string &NavigationScores::getNavigationName() const {
    return navigationName;
}
