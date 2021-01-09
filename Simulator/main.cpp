#include "../Common/Restrictions.h"
#include "Simulation.h"
#include <cstdlib>
#include <dlfcn.h>
#include <string>
#include <filesystem>
#include <iostream>
#include <set>
#include <cassert>

namespace fs = std::filesystem;

int main() {

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
