//
// Created by Tal on 19/11/2020.
//

#include "PointList.h"

#include <utility>
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"
#include "../search/Grid.h"

PointList::PointList(std::vector<Coordinates> &points) : points(std::move(points)), Geometry() {}

const std::vector<Coordinates> &PointList::getPoints() const {
    return points;
}

rapidjson::Value PointList::toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) {
    return geometryJsonSerializer->toJson(this, allocator);
}

std::vector<Grid::GridCell> PointList::getGridCells(const Grid *grid) {
    return grid->getGeometryGridCells(*this);
}



