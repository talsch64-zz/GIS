#pragma once

#include "../GISNamedTypes.h"
#include "../entities/Entity.h"
#include "TopologicalSearch.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <cmath>

class Point;

class PointList;

class Circle;

class TopologicalSearch;

class CellEntities {
    using EntitiesData = std::forward_list<EntityId>;
    EntitiesData entities;

public:
    using const_iterator = EntitiesData::const_iterator;

    void insertEntity(const EntityId &id) {
        entities.emplace_front(id);
    }

    EntitiesData getEntities() {
        return entities;
    }
};


/// Grid stores the GIS entities into GridCells
/// Implemented as a hash-map that maps GridCell to a list of EntityIds of all the entities that runs through the GridCell
/// Each GridCells represents a range of coordinates determined by a pre-defined coordinates precision.
/// Operations supported:
/// * get neighboring cells of a given cell
/// * get all GridCells that a given Geometry runs through them.
/// * set entities with different geometries inside the grid

class Grid {
    std::unique_ptr<TopologicalSearch> topologicalSearch;

public:
    Grid();

    static constexpr double precision = 0.015625;

    static constexpr double meterPrecision = 10;

    using GridCell = Coordinates;

    std::vector<GridCell> setEntityOnGrid(const Entity &entity);

    std::vector<GridCell> getGeometryGridCells(const PointList &geometry) const;

    /* returns all the GridCells which contains the PointList */
    std::vector<GridCell> getGeometryGridCells(const Point &geometry) const;

    /* returns GridCell which contains the Point */
    std::vector<GridCell> getGeometryGridCells(const Circle &geometry) const;

    /* returns all the GridCells which contains the Circle */
    std::vector<GridCell> getCellNeighbors(GridCell initialCell) const;

    /* returns all the neighboring cells of the given GridCell */
    Coordinates truncateCoordinates(const Coordinates &coordinates) const;

    /* returns all the entities inside the given GridCell */
    CellEntities getEntitiesOnGrid(const GridCell &cell);

    void clear();

private:

    std::unordered_map<GridCell, CellEntities> grid;

    double truncateDecimalCoordinate(double coordinate) const;

    /* add all GridCells which the interval between coord1 and coord2 runs through to cells vector */
    void addIntervalsGridCells(const Coordinates &coord1, const Coordinates &coord2,
                               std::unordered_set<GridCell> &cells) const;

    /* returns all the neighboring GridCells of the poll-GridCell
     * if north is true then return the northern polls neighbors, else southern poll neighbors */
    std::vector<GridCell> getPollCellNeighbors(bool north) const;

    /* returns the northern cell - Longitude = 0, Latitude = 90
     * There is only one cell with latitude 90 */
    GridCell getNorthernCell() const;

    /* returns the southrern cell - Longitude = 0, Latitude = -90
    * There is only one cell with latitude -90 */
    GridCell getSouthernCell() const;
};