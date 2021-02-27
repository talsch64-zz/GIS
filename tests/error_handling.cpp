#include "gtest/gtest.h"
#include "../GIS/GIS.h"
#include <fstream>

bool logContainsError(std::string err) {
    std::ifstream infile("errors.log");
    std::string line;
    std::string searchErr = "ERROR: " + err;
    while (std::getline(infile, line)) {
        if (line == searchErr) {
            return true;
        }
    }
    return false;
}

TEST(ErrorHandling, ErrorHandlingTest) {
    GIS gis;

    auto entityIds = gis.loadMapFile("map-with-errors.json");

    ASSERT_EQ((int) entityIds.size(), 1);
    ASSERT_TRUE(logContainsError("Couldn't load invalid entity - JSON entity doesn't contain name"));
    ASSERT_TRUE(logContainsError("Entity with id '1' already exists"));
    ASSERT_TRUE(logContainsError("Couldn't load invalid entity - Invalid coordinate in JSON"));
    ASSERT_TRUE(logContainsError("Couldn't load invalid entity - Way does not contain valid from Junction"));
    ASSERT_TRUE(logContainsError("Couldn't load invalid entity - Circle JSON doesn't contain radius"));
    ASSERT_TRUE(logContainsError("Couldn't load invalid entity - JSON entity doesn't contain geometry"));
}
