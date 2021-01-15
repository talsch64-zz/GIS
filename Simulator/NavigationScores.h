#ifndef GIS_CPP_NAVIGATIONSCORES_H
#define GIS_CPP_NAVIGATIONSCORES_H

#include <vector>

/**
 * The scores of all requests with consensus of a navigation algorithm
 */
class NavigationScores {
    int totalScore;
    std::vector<int> requestScores;
};


#endif //GIS_CPP_NAVIGATIONSCORES_H
