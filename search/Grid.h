#pragma once

#include "../GISdefs.h"
#include "../entities/geometry/Geometry.h"
#include "../entities/geometry/PointList.h"
#include "../entities/geometry/Point.h"
#include "../entities/geometry/Circle.h"
#include "../entities/Entity.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <cmath>


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

    double truncateDecimalCoordinate(double coordinate) const {
        return std::trunc(coordinate / precision) * precision;
    }

    Coordinates truncateCoordinates(const Coordinates &coordinates) const {
        return {Longitude{truncateDecimalCoordinate(coordinates.longitude())},
                Latitude{truncateDecimalCoordinate(coordinates.latitude())}};
    }

public:
    static constexpr double precision = 0.0001;

    using GridCell = Coordinates;
    std::vector<GridCell> setEntityOnGrid(Entity &entity);




private:
    std::unordered_map<GridCell, CellEntities> grid;


    CellEntities getEntitiesOnGrid(const Coordinates &coordinates);
    std::vector<GridCell> getGeometryGridCells(const Geometry &geometry);
    std::vector<Coordinates> getGeometryGridCells(const PointList &geometry);
    std::vector<Coordinates> getGeometryGridCells(const Point &geometry);

    /* add all GridCells which the interval between coord1 and coord2 runs through to cells vector */
    void addIntervalsGridCells(const Coordinates &coord1, const Coordinates &coord2, std::unordered_set<GridCell> &cells);
};