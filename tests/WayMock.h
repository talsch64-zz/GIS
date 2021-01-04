#ifndef GIS_CPP_WAYMOCK_H
#define GIS_CPP_WAYMOCK_H


#include "../GIS/entities/Way.h"

class WayMock : public Way {
public:
    WayMock(const EntityId &id, const std::string &name, const std::string &description,
        const std::vector<std::string> &categoryTags, std::unique_ptr<PointList> geometry, EntityId from,
        EntityId to, TrafficDirection direction, int speedLimit, bool tollRoad, bool highway, std::vector<std::string> restricted);

    void setLength(Meters length);
};


#endif //GIS_CPP_WAYMOCK_H
