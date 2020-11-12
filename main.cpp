#include <iostream>
#include "GIS.h"
#include "rapidjson/document.h"
#include <iostream>
#include <fstream>

int main() {
    GIS gis;
    rapidjson::Document document;
//    const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
//    printf("Original JSON:\n %s\n", json);
//    if (document.Parse(json).HasParseError())
//        return 1;


    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\b.json");

//    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\example1.json");
//        gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\sample-json-file.json");
    return 0;
}
