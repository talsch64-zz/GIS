#include "gtest/gtest.h"
#include "../Common/GISNamedTypes.h"
#include "../Common/CoordinatesMath.h"
#include "../GIS/GIS_315524694.h"

//// Actual naming of test suites and test is up to you
//// However, they should not contain any underscoress (_). See: https://github.com/google/googletest/blob/master/googletest/docs/primer.md
//
TEST(GISBasic, loadMapFile) {
	GIS_315524694 gis;
	std::vector<EntityId> loaded_entities = gis.loadMapFile("nyc.json");
	std::vector<EntityId> expected_entities({EntityId("P1001"), EntityId("P1002"), EntityId("J1001"), EntityId("J1002"), EntityId("J1010"), EntityId("P1010"), EntityId("W2001"), EntityId("W2002")});
	EXPECT_EQ(loaded_entities, expected_entities);
}

TEST(GISBasic, getEntities) {
    GIS_315524694 gis;
	gis.loadMapFile("nyc.json");
	std::vector<EntityId> entities = gis.getEntities("Washington Square Park");
	std::sort(entities.begin(), entities.end());
	std::vector<EntityId> expected_entities({EntityId("P1001")});
    EXPECT_EQ(entities, expected_entities);
}

TEST(GISBasic, clearReturnValue) {
    GIS_315524694 gis;
    gis.loadMapFile("nyc.json");
    std::size_t number_of_cleared_entities = gis.clear();
    std::size_t expected_number_of_cleared_entities = 8;
    EXPECT_EQ(number_of_cleared_entities, expected_number_of_cleared_entities);
}

TEST(GISBasic, saveMapFileReturnValue) {
    GIS_315524694 gis;
    gis.loadMapFile("nyc.json");
	std::size_t number_of_saved_entities = gis.saveMapFile("nyc_updated.json");
	std::size_t expected_number_of_saved_entities = 8;
	EXPECT_EQ(number_of_saved_entities, expected_number_of_saved_entities);
}

