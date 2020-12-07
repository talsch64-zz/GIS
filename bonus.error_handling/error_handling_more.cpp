#include "gtest/gtest.h"
#include "../GIS.h"
#include <fstream>

bool logContainsError(std::string err);

TEST(ErrorHandling, LoadNonExisitingFileTest) {
    GIS gis;

    gis.loadMapFile("map-doesnt-exist.json");

    ASSERT_TRUE(logContainsError("Couldn't load map because the file 'map-doesnt-exist.json' could not be opened"));
}

TEST(ErrorHandling, LoadInvalidJsonTest) {
    GIS gis;

    gis.loadMapFile("bonus.error_handling/invalid-map.json");

    ASSERT_TRUE(logContainsError("The JSON map is invalid"));
}

TEST(ErrorHandling, MixedIdTest) {
    GIS gis;

    auto entityIds = gis.loadMapFile("bonus.error_handling/mixed-id-map.json");

    ASSERT_EQ((int)entityIds.size(), 2);
    ASSERT_TRUE(logContainsError("Couldn't load invalid entity - JSON entity doesn't contain id"));
}