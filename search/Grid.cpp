#include <unordered_set>
#include "Grid.h"
#include "../entities/geometry/Geometry.h"
#include "../CoordinatesMath.h"
#include "../entities/geometry/PointList.h"
#include "../entities/geometry/Point.h"
#include "../entities/geometry/Circle.h"

double Grid::truncateDecimalCoordinate(double coordinate) const {
    return std::floor(coordinate / precision) * precision;
}
Grid::Grid() : topologicalSearch(std::make_unique<TopologicalSearch>()) {
}

Coordinates Grid::truncateCoordinates(const Coordinates &coordinates) const {
//    north poll
    if (coordinates.latitude() == 90) {
        return getNorthernCell();
    }
//    south poll
    if (coordinates.latitude() == -90) {
        return getSouthernCell();
    }
    double longitude = truncateDecimalCoordinate(coordinates.longitude());
    if (longitude == -180) {
        longitude = 180;
    }
    return {Longitude(longitude), Latitude{truncateDecimalCoordinate(coordinates.latitude())}};
}

CellEntities Grid::getEntitiesOnGrid(const GridCell &cell) {
    auto pair = grid.find(cell);
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
    for (int i = 0; i < (int) points.size() - 1; i++) {
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
    return topologicalSearch->searchCircleInGrid(*this, geometry.getCenter(), geometry.getRadius());
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

    Coordinates midPoint = CoordinatesMath::midpoint(coord1, coord2);
    addIntervalsGridCells(coord1, midPoint, cells);
    addIntervalsGridCells(midPoint, coord2, cells);
}


std::vector<Grid::GridCell> Grid::getCellNeighbors(Grid::GridCell initialCell) const {
    double lat = initialCell.latitude();
    double lng = initialCell.longitude();

// initialCell is not a poll cell
    if (lat < 90 && lat > -90) {
//      just to be safe we multiplied precision :)
        double northLat = lat + precision;
        double southLat = lat - precision;
        double eastLng = lng + precision;
        double westLng = lng - precision;

        Coordinates north(truncateCoordinates({Longitude(lng), Latitude(northLat)}));
        Coordinates south(truncateCoordinates({Longitude(lng), Latitude(southLat)}));
        Coordinates east(truncateCoordinates({Longitude(eastLng), Latitude(lat)}));
        Coordinates west(truncateCoordinates({Longitude(westLng), Latitude(lat)}));
        Coordinates northEast(truncateCoordinates({Longitude(eastLng), Latitude(northLat)}));
        Coordinates southEast(truncateCoordinates({Longitude(eastLng), Latitude(southLat)}));
        Coordinates northWest(truncateCoordinates({Longitude(westLng), Latitude(northLat)}));
        Coordinates southWest(truncateCoordinates({Longitude(westLng), Latitude(southLat)}));

        if (northLat == 90) {
//            north = north poll cell
            return std::vector<GridCell>{east, west, southEast, southWest, south, getNorthernCell()};
        } else if (southLat == -90) {
//            south = south poll cell
            return std::vector<GridCell>{east, west, northEast, northWest, north, getSouthernCell()};
        }
        return std::vector<GridCell>{north, south, east, west, northEast, southEast, northWest, southWest};
    }
    return getPollCellNeighbors(lat == 90);
}


std::vector<Grid::GridCell> Grid::getPollCellNeighbors(bool north) const {
    std::vector<GridCell> neighbors;
    double lng = 0;
    double lat = 90 - precision;
    if (!north) lat = -lat;
    while (lng < 360) {
        neighbors.emplace_back(Longitude(lng), Latitude(lat));
        lng = truncateDecimalCoordinate(lng + precision);
    }
    return neighbors;
}

Grid::GridCell Grid::getNorthernCell() const {
    return Coordinates(Longitude(0), Latitude(90));
}

Grid::GridCell Grid::getSouthernCell() const {
    return Coordinates(Longitude(0), Latitude(90));
}

void Grid::clear() {
    grid.clear();
}
