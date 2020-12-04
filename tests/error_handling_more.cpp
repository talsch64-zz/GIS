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

    gis.loadMapFile("invalid-map.json");

    ASSERT_TRUE(logContainsError("The JSON map is invalid"));
}

TEST(ErrorHandling, MixedIdTest) {
    GIS gis;

    auto entityIds = gis.loadMapFile("mixed-id-map.json");

    ASSERT_EQ(entityIds.size(), 2);
    ASSERT_TRUE(logContainsError("Couldn't load invalid entity - JSON entity doesn't contain id"));
}