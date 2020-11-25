#pragma once

#include "../GISdefs.h"
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
    std::unique_ptr<TopologicalSearch> topologicalSearch;

public:
    Grid();

    static constexpr double precision = 0.01;

    using GridCell = Coordinates;

    CellEntities getEntitiesOnGrid(const Coordinates &coordinates);

    std::vector<GridCell> setEntityOnGrid(const Entity &entity);

//    TODO remove after testing
//    std::vector<GridCell> setEntityOnGrid(const PointList &geometry, const EntityId &id);
//    std::vector<Grid::GridCell> setEntityOnGrid(const Point &geometry, const EntityId &id);
    std::vector<GridCell> getGeometryGridCells(const PointList &geometry) const;

    std::vector<GridCell> getGeometryGridCells(const Point &geometry) const;

    std::vector<GridCell> getGeometryGridCells(const Circle &geometry) const;

private:
    std::unordered_map<GridCell, CellEntities> grid;


    /* add all GridCells which the interval between coord1 and coord2 runs through to cells vector */
    void
    addIntervalsGridCells(const Coordinates &coord1, const Coordinates &coord2,
                          std::unordered_set<GridCell> &cells) const;

    double truncateDecimalCoordinate(double coordinate) const {
        return std::trunc(coordinate / precision) * precision;
    }

    Coordinates truncateCoordinates(const Coordinates &coordinates) const {
        return {Longitude{truncateDecimalCoordinate(coordinates.longitude())},
                Latitude{truncateDecimalCoordinate(coordinates.latitude())}};
    }
};