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
        //TODO handle errors if arguments are wrong or not representing absolute path
        switch (opt) {
            case 't' :
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
                //TODO handle error (maybe print usage message)
                exit(EXIT_FAILURE);
        }

    }
    //TODO handle error if one of the mandatory fields is missing!

}

void Registrar::loadSharedLibraries() {
    loadGISLibraries();
    loadNavigationLibraries();
}

void Registrar::unloadSharedLibraries() {
    for (auto &so_file: so_files) {
        std::cout << "unloaded so_file" << std::endl;
        dlclose(so_file);

    }
}

void Registrar::loadGISLibraries() {
    Simulation &simulation = Simulation::getInstance();
    for (auto &gis_so: std::filesystem::directory_iterator(gis_directory)) {
        //TODO check that the file is not a directory and with .so extension
        std::string nextName = gis_so.path().stem();
        simulation.setNextName(nextName);
        void *gis_handle = dlopen(gis_so.path().c_str(), RTLD_LAZY);
        if (gis_handle) {
            so_files.push_back(gis_handle);
            //TODO delete
            std::cout << gis_so << " loaded" << std::endl;
        } else {
            //TODO handle error
            std::cout << dlerror() << std::endl;
        }
    }
}

void Registrar::loadNavigationLibraries() {
    Simulation &simulation = Simulation::getInstance();
    for (auto &navigation_so: std::filesystem::directory_iterator(navigation_directory)) {
        //TODO check that the file is not a directory and with .so extension
        std::string nextName = navigation_so.path().stem();
        simulation.setNextName(nextName);
        void *navigation_handle = dlopen(navigation_so.path().c_str(), RTLD_LAZY);
        if (navigation_handle) {
            so_files.push_back(navigation_handle);
            //TODO delete
            std::cout << navigation_so << " loaded" << std::endl;
        } else {
            //TODO handle error
            std::cout << dlerror() << std::endl;
        }
    }
}






