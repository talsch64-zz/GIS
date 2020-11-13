#include <iostream>
#include "GIS.h"
#include "rapidjson/document.h"
#include <iostream>
#include <fstream>

int main() {
    GIS gis;
    rapidjson::Document document;
    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\poi.json");
    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\junction.json");
    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\way.json");
    return 0;
}
