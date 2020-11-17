#ifndef EX1_COORDINATES_H
#define EX1_COORDINATES_H


#include "../../GISdefs.h"

class Coordinates {
    Longitude _longitude;
    Latitude _latitude;
public:
    Coordinates(Longitude longitude, Latitude latitude) : _longitude(longitude), _latitude(latitude) {}

    Longitude longitude() const { return _longitude; }

    Latitude latitude() const { return _latitude; }

    bool operator==(const Coordinates &rhs) const {
        return _longitude == rhs._longitude && _latitude == rhs._latitude;
    }
};


#endif //EX1_COORDINATES_H
