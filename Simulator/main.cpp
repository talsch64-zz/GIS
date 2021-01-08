#include "../Common/Restrictions.h"
#include "Simulation.h"
#include <cstdlib>
#include <dlfcn.h>
#include <string>
#include <filesystem>
#include <iostream>
#include <set>
#include <cassert>
#include <unistd.h>
#include <getopt.h>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    int opt = 0, option_index = 0, num_threads = 1;
    std::filesystem::path navigation_path, gis_path, map_path, requests_path, output_path;


    static struct option long_options[] = {
            {"num_threads",         optional_argument, 0, 't'},
            {"navigation",          required_argument, 0, 'n'},
            {"gis",                 required_argument, 0, 'g'},
            {"map_file",            required_argument, 0, 'm'},
            {"navigation_requests", required_argument, 0, 'r'},
            {"output",              optional_argument, 0, 'o'},
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
                navigation_path = optarg;
                break;
            case 'g' :
                gis_path = optarg;
                break;
            case 'm' :
                map_path = optarg;
                break;
            case 'r':
                requests_path = optarg;
                break;
            case 'o':
                output_path = optarg;
                break;
            default:
                //TODO handle error (maybe print usage message)
                exit(EXIT_FAILURE);
        }

    }

    Simulation &simulation = Simulation::getInstance();

    for(auto& navigation_so: std::filesystem::directory_iterator(navigation_path)) {
        //TODO check that the file is not a directory and with .so extension
        std::string nextName = navigation_so.path().stem();
        simulation.setNextName(nextName);
        void *navigation_handle = dlopen(navigation_so.path().c_str(), RTLD_LAZY);
        if (navigation_handle) {
            std::cout << navigation_so << " loaded" << std::endl;
        } else {
            std::cout << dlerror() << std::endl;
        }
    }





    // dlopen usage example:
    fs::path run_dir = fs::current_path();

    std::string gisName = "GIS_315524694";
    std::string gisPath = "GIS/" + gisName + ".so";
    fs::path gis_so{run_dir / gisPath};
    Simulation &sim = Simulation::getInstance();
    sim.setNextName(gisName);
    void *gis_handle = dlopen(gis_so.c_str(), RTLD_LAZY);
    if (gis_handle) {
        std::cout << gis_so << " loaded" << std::endl;
    } else {
        std::cout << dlerror() << std::endl;
        return 1;
    }

    std::string navigationName = "Navigation_315524694";
    std::string navigationPath = "Navigation/" + navigationName + ".so";
    fs::path navigation_so{run_dir / navigationPath};
    sim.setNextName(navigationName);
    void *navigation_handle = dlopen(navigation_so.c_str(), RTLD_LAZY);
    if (navigation_handle) {
        std::cout << navigation_so << " loaded\n";
    } else {
        std::cout << dlerror() << std::endl;
        return 1;
    }

    // Don't forget to close all handles!
    dlclose(gis_handle);
    dlclose(navigation_handle);

    // Restriction usage example:
    Restrictions r1(" toll ,highway");
    assert(r1.contains("toll"));
    assert(r1.contains("highway"));
    assert(!r1.contains("sideroad"));
    assert(r1.getRestrictions().size() == 2);
    assert(*(r1.getRestrictions().begin()) == "toll" || *(r1.getRestrictions().begin()) == "highway");

    std::string res = "long way, short way ";
    Restrictions r2(res);
    assert(r2.contains("long way"));
    assert(r2.contains("short way"));
    assert(!r2.contains("highway"));
    std::set<std::string> check = {"long way", "short way"};
    for (auto rstr: r2.getRestrictions()) {
        assert(check.erase(rstr) == 1);
    }
    assert(check.size() == 0);

    Restrictions r3(",");
    assert(!r3.contains(""));
    assert(r3.getRestrictions().size() == 0);
    assert(r3.getRestrictions().begin() == r3.getRestrictions().end());

    std::cout << "end of main" << std::endl;

    return EXIT_SUCCESS;
}
