#ifndef GIS_CPP_ROUTEMOCK_H
#define GIS_CPP_ROUTEMOCK_H


#include "../navigation/Route.h"

class RouteMock: public Route {
public:
    void setLength(Meters length);
    void setDuration(Minutes duration);
    void setWays(std::vector<std::pair<EntityId, Direction>> ways);
    void setValid(bool valid);
};


#endif //GIS_CPP_ROUTEMOCK_H
