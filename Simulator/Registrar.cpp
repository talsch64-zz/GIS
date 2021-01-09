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
                //TODO check if number is valid string-number because atoi ignores non digit chars.
                num_threads = atoi(optarg);
                break;
            case 'n' :
                navigation_directory = optarg;
                break;
            case 'g' :
                gis_directory = optarg;
                break;
            case 'm' :
                map_file = optarg;
                break;
            case 'r':
                navigation_requests = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            default:
                printUsage(argv[0]);
        }
    }
    if (validateCommandLineArguments() == FAILURE) {
        printUsage(argv[0]);
    }
}

void Registrar::loadSharedLibraries() {
    loadGISLibraries();
    loadNavigationLibraries();
}

void Registrar::unloadSharedLibraries() {
    for (auto &so_file: gis_so_files) {
        dlclose(so_file);
    }
    for (auto &so_file: navigation_so_files) {
        dlclose(so_file);
    }
}

void Registrar::loadGISLibraries() {
    Simulation &simulation = Simulation::getInstance();
    for (auto &gis_so: std::filesystem::directory_iterator(gis_directory)) {
        std::string nextName = gis_so.path().stem();
        simulation.setNextName(nextName);
        if (gis_so.is_directory() || gis_so.path().extension() != SO_EXTENSION) {
            std::cout << "ERROR: invalid GIS shared library" << std::endl;
            continue;
        }
        void *gis_handle = dlopen(gis_so.path().c_str(), RTLD_LAZY);
        if (gis_handle) {
            gis_so_files.push_back(gis_handle);
        } else {
            std::cout << dlerror() << std::endl;
        }
    }

    if (gis_so_files.empty()) {
        std::cout << "ERROR: Unable to load gis shared objects" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Registrar::loadNavigationLibraries() {
    Simulation &simulation = Simulation::getInstance();
    for (auto &navigation_so: std::filesystem::directory_iterator(navigation_directory)) {
        std::string nextName = navigation_so.path().stem();
        simulation.setNextName(nextName);
        if (navigation_so.is_directory() || navigation_so.path().extension() != SO_EXTENSION) {
            std::cout << "ERROR: invalid GIS shared library" << std::endl;
            continue;
        }
        void *navigation_handle = dlopen(navigation_so.path().c_str(), RTLD_LAZY);
        if (navigation_handle) {
            navigation_so_files.push_back(navigation_handle);
        } else {
            std::cout << dlerror() << std::endl;
        }
    }
    if (navigation_so_files.empty()) {
        std::cout << "ERROR: Unable to load navigation shared objects" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Registrar::printUsage(char *progName) {
    std::cout << progName << "[options]" << std::endl << "Options:" << std::endl <<
              "-num_threads             number of threads to run the simulation. The value must be at least 3. If flag is not specified only 1 thread is created."
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
    if (num_threads <= 0 || num_threads == 2) {
        std::cerr << "ERROR: Invalid num_theards" << std::endl;
        return FAILURE;
    }

    if (!fs::exists(navigation_directory) || navigation_directory.is_relative() ||
        !fs::is_directory(navigation_directory)) {
        std::cerr << "ERROR: Invalid navigation directory: " << navigation_directory << std::endl;
        return FAILURE;
    }
    if (!fs::exists(navigation_directory) || gis_directory.is_relative() || !fs::is_directory(gis_directory)) {
        std::cerr << "ERROR: Invalid GIS directory: " << gis_directory << std::endl;
        return FAILURE;
    }

    if (!fs::exists(map_file) || map_file.is_relative() || fs::is_directory(map_file)) {
        std::cerr << "ERROR: Invalid map file: " << map_file << std::endl;
        return FAILURE;
    }

    if (!fs::exists(navigation_requests) || navigation_requests.is_relative() ||
        fs::is_directory(navigation_requests)) {
        std::cerr << "ERROR: Invalid navigation requests file: " << navigation_requests << std::endl;
        return FAILURE;
    }

    if (!output.empty() && (!fs::exists(output) || output.is_relative() || fs::is_directory(output))) {
        // if output was given as an argument and is not valid
        std::cerr << "ERROR: Invalid output file: " << output << std::endl;
        return FAILURE;
    }
    return SUCCESS;
}






