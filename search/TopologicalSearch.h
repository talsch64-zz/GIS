#ifndef EX1_TOPOLOGICALSEARCH_H
#define EX1_TOPOLOGICALSEARCH_H


#include <vector>
#include <memory>
#include "../GISdefs.h"
#include "Grid.h"

class TopologicalSearch {
    std::vector<Coordinates> searchCircleInGrid(const Coordinates &center, Meters radius);
    std::shared_ptr<Grid> grid;
public:
    TopologicalSearch(std::shared_ptr<Grid> grid);
};


#endif //EX1_TOPOLOGICALSEARCH_H
