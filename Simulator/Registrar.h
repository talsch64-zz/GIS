

#ifndef GIS_CPP_REGISTRAR_H
#define GIS_CPP_REGISTRAR_H


#include <vector>
#include <string>
#include <filesystem>

class Registrar {
    std::vector<void *> gis_so_files;
    std::vector<void *> navigation_so_files;
    std::filesystem::path navigation_directory;
    std::filesystem::path gis_directory;
    std::filesystem::path map_file;
    std::filesystem::path navigation_requests;
    std::filesystem::path output; //TODO handle the case where no output is given
    int num_threads = 1;


public:
    void parseCommandLineArguments(int argc, char **argv);

    void loadSharedLibraries();

    void unloadSharedLibraries();

    void printUsage(char *progName);


private:

    void loadGISLibraries();

    void loadNavigationLibraries();

    bool validateCommandLineArguments();

};

#endif //GIS_CPP_REGISTRAR_H

