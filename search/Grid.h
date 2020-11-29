#pragma once

#include "../GISdefs.h"
#include "../entities/Entity.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <cmath>

class Point;

class PointList;

class Circle;

class CellEntities {
    //TODO: consider keeping way's segments separated from all other entities
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

class Grid {

public:
    static constexpr double precision = 0.015625;
    static constexpr double meterPrecision = 1;

    using GridCell = Coordinates;

//TODO maybe change to return unordered_set
    std::vector<GridCell> setEntityOnGrid(const Entity &entity);

//    TODO remove after testing
//    std::vector<GridCell> setEntityOnGrid(const PointList &geometry, const EntityId &id);
//    std::vector<Grid::GridCell> setEntityOnGrid(const Point &geometry, const EntityId &id);
    std::vector<GridCell> getGeometryGridCells(const PointList &geometry) const;

    std::vector<GridCell> getGeometryGridCells(const Point &geometry) const;

    std::vector<GridCell> getGeometryGridCells(const Circle &geometry) const;

    std::vector<GridCell> getCellNeighbors(GridCell initialCell);

    Coordinates truncateCoordinates(const Coordinates &coordinates) const;

    CellEntities getEntitiesOnGrid(const GridCell &cell);


private:
    std::unordered_map<GridCell, CellEntities> grid;


    double truncateDecimalCoordinate(double coordinate) const;
    /* add all GridCells which the interval between coord1 and coord2 runs through to cells vector */
    void addIntervalsGridCells(const Coordinates &coord1, const Coordinates &coord2, std::unordered_set<GridCell> &cells) const;
    std::vector<GridCell> getPollCellNeighbors(bool north);
    GridCell getNorthernCell() const;
    GridCell getSouthernCell() const;
};