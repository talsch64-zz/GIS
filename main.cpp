#include <iostream>
#include "GIS.h"
#include "rapidjson/document.h"
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <winbase.h>

std::string getMapFilePath(const std::string &name);

int main() {
    GIS gis;
    rapidjson::Document document;


    std::string mapPath = getMapFilePath("a.json");
    gis.loadMapFile(mapPath);
//    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\poi.json")
//    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\junction.json");
//    gis.loadMapFile("C:\\Users\\Tal\\Desktop\\university\\CS\\year3\\01 - cpp\\03 - Project\\ex1\\way.json");
    return 0;
}

std::string getMapFilePath(const std::string &name) {
    static TCHAR buffer[MAX_PATH];
    static bool assigned = false;
    if (!assigned) {
        SetCurrentDirectory(_T(".."));
        GetCurrentDirectory(MAX_PATH, buffer);
        assigned = true;
    }
    std::string path = std::string(buffer) + "\\json-files\\" + name;
    return path;
}
