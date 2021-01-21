//
// Created by student on 08/01/2021.
//

#include "Registrar.h"
#include "Simulation.h"
#include <getopt.h>
#include <cstdlib>
#include <dlfcn.h>
#include <string>
#include <iostream>
#include <set>
#include <cassert>

#define SUCCESS 0
#define FAILURE 1
#define SO_EXTENSION ".so"

void Registrar::parseCommandLineArguments(int argc, char **argv) {
    int opt = 0, option_index = 0;
    static struct option long_options[] = {
            {"num_threads",         required_argument, 0, 't'},
            {"navigation",          required_argument, 0, 'n'},
            {"gis",                 required_argument, 0, 'g'},
            {"map_file",            required_argument, 0, 'm'},
            {"navigation_requests", required_argument, 0, 'r'},
            {"output",              required_argument, 0, 'o'},
            {0, 0,                                     0, 0}
    };

    while ((opt = getopt_long_only(argc, argv, "",
                                   long_options, &option_index)) != -1) {
        switch (opt) {
            case 't' :
                numThreads = parseNumber(optarg);
                break;
            case 'n' :
                navigationDirectory = optarg;
                break;
            case 'g' :
                gisDirectory = optarg;
                break;
            case 'm' :
                mapFile = optarg;
                break;
            case 'r':
                navigationRequests = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            default:
                printUsage(argv[0]);
        }
    }
    //reset
    optind = 1;

    if (validateCommandLineArguments() == FAILURE) {
        printUsage(argv[0]);
    }
}

void Registrar::loadSharedLibraries() {
    loadGISLibraries();
    loadNavigationLibraries();
}

void Registrar::unloadSharedLibraries() {
    for (auto &so_file: gisSoFiles) {
        dlclose(so_file);
    }
    for (auto &so_file: navigationSoFiles) {
        dlclose(so_file);
    }
}

void Registrar::loadGISLibraries() {
    Simulation &simulation = Simulation::getInstance();
    for (auto &gis_so: std::filesystem::directory_iterator(gisDirectory)) {
        std::string nextName = gis_so.path().stem();
        simulation.setNextName(nextName);
        if (gis_so.is_directory() || gis_so.path().extension() != SO_EXTENSION) {
            std::cout << "ERROR: found none SO file in GIS shared library directory" << std::endl;
            continue;
        }
        void *gis_handle = dlopen(gis_so.path().c_str(), RTLD_LAZY);
        if (gis_handle) {
            gisSoFiles.push_back(gis_handle);
        } else {
            std::cout << dlerror() << std::endl;
        }
    }

    if (gisSoFiles.empty()) {
        std::cout << "ERROR: Unable to load gis shared objects" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Registrar::loadNavigationLibraries() {
    Simulation &simulation = Simulation::getInstance();
    for (auto &navigation_so: std::filesystem::directory_iterator(navigationDirectory)) {
        std::string nextName = navigation_so.path().stem();
        simulation.setNextName(nextName);
        if (navigation_so.is_directory() || navigation_so.path().extension() != SO_EXTENSION) {
            std::cout << "ERROR: found none SO file in Navigation shared library directory" << std::endl;
            continue;
        }
        void *navigation_handle = dlopen(navigation_so.path().c_str(), RTLD_LAZY);
        if (navigation_handle) {
            navigationSoFiles.push_back(navigation_handle);
        } else {
            std::cout << dlerror() << std::endl;
        }
    }
    if (navigationSoFiles.empty()) {
        std::cout << "ERROR: Unable to load navigation shared objects" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Registrar::printUsage(char *progName) {
    std::cout << progName << "[options]" << std::endl << "Options:" << std::endl <<
              "-num_threads             number of threads to run the simulation. The value must be at least 3. If flag is not specified default is 1."
              << std::endl <<
              "-navigation              Navigation shared objects directory path." << std::endl <<
              "-gis                     GIS shared libraries directory path." << std::endl <<
              "-map_file                Map file path." << std::endl <<
              "-navigation_requests     Navigation requests file path." << std::endl <<
              "-output                  Output filepath. If flag is not specified, output.txt is automatically created in program's directory."
              << std::endl;

    exit(EXIT_FAILURE);

}

bool Registrar::validateCommandLineArguments() {
    namespace fs = std::filesystem;
    if (numThreads <= 0 || numThreads == 2) {
        std::cerr << "ERROR: Invalid num_theards argument" << std::endl;
        return FAILURE;
    }

    if (!fs::exists(navigationDirectory) || navigationDirectory.is_relative() ||
        !fs::is_directory(navigationDirectory)) {
        std::cerr << "ERROR: Invalid navigation directory: " << navigationDirectory << std::endl;
        return FAILURE;
    }
    if (!fs::exists(navigationDirectory) || gisDirectory.is_relative() || !fs::is_directory(gisDirectory)) {
        std::cerr << "ERROR: Invalid GIS directory: " << gisDirectory << std::endl;
        return FAILURE;
    }

    if (!fs::exists(mapFile) || mapFile.is_relative() || fs::is_directory(mapFile)) {
        std::cerr << "ERROR: Invalid map file: " << mapFile << std::endl;
        return FAILURE;
    }

    if (!fs::exists(navigationRequests) || navigationRequests.is_relative() ||
        fs::is_directory(navigationRequests)) {
        std::cerr << "ERROR: Invalid navigation requests file: " << navigationRequests << std::endl;
        return FAILURE;
    }

    if (!output.empty() && (!fs::exists(output) || output.is_relative() || !fs::is_directory(output))) {
        // if output was given as an argument and is not valid
        std::cerr << "ERROR: Invalid output directory: " << output << std::endl;
        return FAILURE;
    }
    return SUCCESS;
}

const std::filesystem::path &Registrar::getMapFilePath() const {
    return mapFile;
}

const std::filesystem::path &Registrar::getNavigationRequestsPath() const {
    return navigationRequests;
}

const std::filesystem::path &Registrar::getOutputPath() const {
    return output;
}

int Registrar::getNumThreads() const {
    return numThreads;
}

int Registrar::parseNumber(char *str) {
    for (std::size_t i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return std::stoi(str);
}






