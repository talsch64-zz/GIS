//
// Created by student on 08/01/2021.
//

#ifndef GIS_CPP_REGISTRAR_H
#define GIS_CPP_REGISTRAR_H


#include <vector>
#include <string>
#include <filesystem>

class Registrar {
    std::vector<std::filesystem::path> gis_so_files;
    std::vector<std::filesystem::path> navigation_so_files;
    std::filesystem::path navigation_directory;
    std::filesystem::path gis_directory;
    std::filesystem::path map_file;
    std::filesystem::path navigation_requests;
    std::filesystem::path output;
    int num_threads = 1;


public:

    void parseCommandLineArguments(int argc, char **argv);

    void registerGISLibraries();

    void registerNavigationLibraries();



#endif //GIS_CPP_REGISTRAR_H
