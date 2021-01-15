#ifndef GIS_CPP_NAVIGATIONSCORES_H
#define GIS_CPP_NAVIGATIONSCORES_H

#include <vector>
#include <string>

/**
 * The scores of all requests with consensus of a navigation algorithm
 */
class NavigationScores {
    std::string navigationName;
    int totalScore = 0;
    std::vector<int> requestScores;

public:
    NavigationScores(const std::string &navigationName);

    void addScore(int score);
};


#endif //GIS_CPP_NAVIGATIONSCORES_H
