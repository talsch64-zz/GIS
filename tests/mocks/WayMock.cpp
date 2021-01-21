#include "WayMock.h"

WayMock::WayMock(const EntityId &id, const std::string &name, const std::string &description,
                 const std::vector<std::string> &categoryTags, std::unique_ptr<PointList> geometry, EntityId from,
                 EntityId to, TrafficDirection direction, int speedLimit, bool tollRoad, bool highway,
                 std::vector<std::string> restricted) : Way(id, name, description, categoryTags,
                                                            std::move(geometry),
                                                            from, to, direction, speedLimit,
                                                            tollRoad,
                                                            highway, restricted) {

}

void WayMock::setLength(Meters length) {
    this->length = length;
}

