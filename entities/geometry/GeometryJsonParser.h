
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
    std::unique_ptr<Circle> parseCircle(rapidjson::Value &value);

    std::unique_ptr<Polygon> parsePolygon(rapidjson::Value &value);

    std::vector<Coordinates> parseCoordinates(rapidjson::Value &value);

public:
    std::unique_ptr<Geometry> parseGeometry(rapidjson::Value &doc);
};

#endif //EX1_GEOMETRYJSONPARSER_H
