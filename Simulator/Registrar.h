

#ifndef GIS_CPP_REGISTRAR_H
#define GIS_CPP_REGISTRAR_H


#include <vector>
#include <string>
#include <filesystem>


/**
 * class Registrar handles .so registration and command line arguments parsing.
 *
 */
class Registrar {
    std::vector<void *> gisSoFiles;
    std::vector<void *> navigationSoFiles;
    std::filesystem::path navigationDirectory;
    std::filesystem::path gisDirectory;
    std::filesystem::path mapFile;
    std::filesystem::path navigationRequests;
    std::filesystem::path output; //TODO handle the case where no output is given
    int numThreads = 1;

public:

    void parseCommandLineArguments(int argc, char **argv);

    void loadSharedLibraries();

    void unloadSharedLibraries();

    void printUsage(char *progName);

    const std::filesystem::path &getMapFilePath() const;

    const std::filesystem::path &getNavigationRequestsPath() const;

    const std::filesystem::path &getOutputPath() const;

    int getNumThreads() const;

private:

    void loadGISLibraries();

    void loadNavigationLibraries();

    bool validateCommandLineArguments();

};

#endif //GIS_CPP_REGISTRAR_H

