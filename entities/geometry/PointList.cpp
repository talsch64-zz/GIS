//
// Created by Tal on 19/11/2020.
//

#include "PointList.h"

#include <utility>

PointList::PointList(std::vector<Point> points) : Geometry(), points(std::move(points)) {}

const std::vector<Point> &PointList::getPoints() const {
    return points;
}

void PointList::addPoint(Point p) {
    points.push_back(p);
}


