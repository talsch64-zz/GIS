#ifndef GIS_CPP_NAVIGATIONREQUEST_H
#define GIS_CPP_NAVIGATIONREQUEST_H

#include "../Common/GISNamedTypes.h"

class NavigationRequest {
    Coordinates from;
    Coordinates to;

public:
    NavigationRequest(const Coordinates &from, const Coordinates &to);

    const Coordinates &getFrom() const;

    const Coordinates &getTo() const;

    std::string toString() const;
};


#endif //GIS_CPP_NAVIGATIONREQUEST_H
