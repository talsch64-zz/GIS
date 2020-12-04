#include <iostream>
#include "GIS.h"
#include "entities/JsonHandlers/Serializers/EntityJsonSerializer.h"
#include "entities/geometry/CoordinatesMath.h"
#include "entities/geometry/Circle.h"
#include <iostream>
#include <fstream>


int main(int argc, char *argv[]) {
    GIS gis;
    gis.loadMapFile("something.json");

    return 0;
}