#include <unordered_set>
#include "Grid.h"
#include "../entities/geometry/Geometry.h"
#include "../entities/geometry/CoordinatesMath.h"
#include "../entities/geometry/PointList.h"
#include "../entities/geometry/Point.h"
#include "../entities/geometry/Circle.h"

double Grid::truncateDecimalCoordinate(double coordinate) const {
    return std::floor(coordinate / precision) * precision;
}

Coordinates Grid::truncateCoordinates(const Coordinates &coordinates) const {
    return {Longitude{truncateDecimalCoordinate(coordinates.longitude())},
            Latitude{truncateDecimalCoordinate(coordinates.latitude())}};
}

CellEntities Grid::getEntitiesOnGrid(const Coordinates &coordinates) {
    GridCell grid_cell = truncateCoordinates(coordinates);
    auto pair = grid.find(grid_cell);
    if (pair == grid.end()) {
        return CellEntities();
    } else {
        return pair->second;
    }
}

std::vector<Grid::GridCell> Grid::setEntityOnGrid(const Entity &entity) {
    Geometry &entityGeometry = *(entity.getGeometry().get());
    std::vector<Grid::GridCell> cells = entityGeometry.getGridCells(this);
    EntityId id = entity.getId();
    for (const auto &cell: cells) {
        grid[cell].insertEntity(id);
    }
    return cells;
}

std::vector<Coordinates> Grid::getGeometryGridCells(const PointList &geometry) const {
    std::vector<Coordinates> points = geometry.getPoints();
    std::unordered_set<GridCell> cellSet;
    for (int i = 0; i < points.size() - 1; i++) {
        addIntervalsGridCells(points[i], points[i + 1], cellSet);
    }
    std::vector<GridCell> cells;
    cells.reserve(cellSet.size());
    for (const auto &cell: cellSet) {
        cells.push_back(cell);
    }
    return cells;
}

std::vector<Grid::GridCell> Grid::getGeometryGridCells(const Point &geometry) const {
    return std::vector<Grid::GridCell>{truncateCoordinates(geometry.getCoordinates())};
}

std::vector<Grid::GridCell> Grid::getGeometryGridCells(const Circle &geometry) const {
    return std::vector<GridCell>();
}

void Grid::addIntervalsGridCells(const Coordinates &coord1, const Coordinates &coord2,
                            std::unordered_set<GridCell> &cells) const {
    GridCell cell1 = truncateCoordinates(coord1);
    GridCell cell2 = truncateCoordinates(coord2);
    if (cell1 == cell2 || CoordinatesMath::calculateDistance(coord1, coord2) < meterPrecision) {
        cells.insert(cell1);
        cells.insert(cell2);
        return;
    }

    Coordinates midPoint = CoordinatesMath::calculateMidpoint(coord1, coord2);
    addIntervalsGridCells(coord1, midPoint, cells);
    addIntervalsGridCells(midPoint, coord2, cells);
}

std::vector<Grid::GridCell> Grid::getFrameCells(Grid::GridCell initialCell, int frameDistance) {

    double bl_lng = initialCell.longitude() - precision * frameDistance;
    double bl_lat = initialCell.latitude() - precision * frameDistance;

    double br_lng = initialCell.longitude() + precision * frameDistance;
    double br_lat = initialCell.latitude() - precision * frameDistance;

    double tr_lng = initialCell.longitude() + precision * frameDistance;
    double tr_lat = initialCell.latitude() + precision * frameDistance;

    double tl_lng = initialCell.longitude() - precision * frameDistance;
    double tl_lat = initialCell.latitude() + precision * frameDistance;







    return std::vector<GridCell>();
}
