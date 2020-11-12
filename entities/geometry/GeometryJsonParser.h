
#ifndef EX1_GEOMETRYJSONPARSER_H
#define EX1_GEOMETRYJSONPARSER_H


#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include "Geometry.h"
#include "Circle.h"
#include "Polygon.h"
#include "../CoordinatesParser.h"



class GeometryJsonParser {
private:
    Circle parseCircle(rapidjson::Value &value);

    Geometry parsePolygon(rapidjson::Value &value);

//    void getCoordinates(rapidjson::Value &value, std::vector<Coordinates> &coordinates);

public:
    Geometry parseGeometry(rapidjson::Value &doc);
};

#endif //EX1_GEOMETRYJSONPARSER_H
