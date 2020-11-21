//
// Created by Tal on 19/11/2020.
//

#include "PointList.h"

#include <utility>

PointList::PointList(const std::vector<Coordinates> &points) : points(points) {}

const std::vector<Coordinates> &PointList::getPoints() const {
    return points;
}

void PointList::addPoint(Coordinates p) {
    points.push_back(p);
}

rapidjson::Value PointList::toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) {
    return rapidjson::Value();
}



