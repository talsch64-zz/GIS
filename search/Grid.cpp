#include <unordered_set>
#include "Grid.h"
#include "../entities/geometry/Geometry.h"
#include "../entities/geometry/CoordinatesMath.h"


void Grid::addIntervalsGridCells(const Coordinates &coord1, const Coordinates &coord2, std::unordered_set<GridCell> &cells) {
    if(abs(coord1.latitude()-coord2.latitude()) < Grid::precision && abs(coord1.latitude()-coord2.latitude()) < Grid::precision) {
        cells.insert(truncateCoordinates(coord1));
        cells.insert(truncateCoordinates(coord2));
        return;
    }
    Coordinates midPoint = CoordinatesMath::calculateMidpoint(coord1, coord2);
    addIntervalsGridCells(coord1, midPoint, cells);
    addIntervalsGridCells(midPoint, coord2, cells);
}

//std::vector<Coordinates> Grid::searchPointListGridCells(PointList &geometry) {

std::vector<Coordinates> Grid::searchPointListGridCells(PointList &geometry) {
    std::vector<Coordinates> points = geometry.getPoints();
    std::unordered_set<GridCell> cellSet;
    for (int i = 0; i < points.size() - 1; i++) {
        addIntervalsGridCells(points[i], points[i + 1], cellSet);
    }
    std::vector<GridCell> cells;
    for (const auto& cell: cellSet) {
        cells.push_back(cell);
    }
    return cells;
}

std::vector<Grid::GridCell> Grid::setEntityOnGrid(PointList &geometry, const EntityId &id) {
    std::vector<GridCell> cells = searchPointListGridCells(geometry);
    for(const auto& cell: cells) {
        grid[cell].insertEntity(id);
    }
    return cells;
}

std::vector<Grid::GridCell> Grid::setEntityOnGrid(Point &geometry, const EntityId &id) {
    std::vector<GridCell> gridCells;
    GridCell cell = truncateCoordinates(geometry.getCoordinates());
    grid[cell].insertEntity(id);
    gridCells.emplace_back(cell);
    return gridCells;
}
