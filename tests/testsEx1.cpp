

#include "gtest/gtest.h"
#include "../GISProvider.h"
#include "../CoordinatesMath.h"


std::vector<EntityId> nyc_entities(
        {EntityId("P1001"), EntityId("P1002"), EntityId("J1001"), EntityId("J1002"), EntityId("J1010"),
         EntityId("P1010"), EntityId("W2001"), EntityId("W2002")});

TEST(GISPre, loadMapFile) {
    GIS gis = GISProvider::getGIS();
    std::vector<EntityId> loaded_entities = gis.loadMapFile("nyc.json");
    EXPECT_EQ(loaded_entities, nyc_entities);
}

TEST(GISPre, getEntities) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("nyc.json");
    std::vector<EntityId> entities = gis.getEntities("Washington Square Park");
    std::sort(entities.begin(), entities.end());
    std::vector<EntityId> expected_entities({EntityId("P1001")});
    EXPECT_EQ(entities, expected_entities);
}

TEST(GISPRe, clearReturnValue) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("nyc.json");
    std::size_t number_of_cleared_entities = gis.clear();
    EXPECT_EQ(number_of_cleared_entities, nyc_entities.size());
}

TEST(GISPre, saveMapFileReturnValue) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("nyc.json");
    std::size_t number_of_saved_entities = gis.saveMapFile("nyc_updated.json");
    EXPECT_EQ(number_of_saved_entities, nyc_entities.size());
}

TEST(GISEx1, clearLoad) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("nyc.json");
    gis.clear();
    std::vector<EntityId> loaded_entities = gis.loadMapFile("nyc.json");
    EXPECT_EQ(loaded_entities, nyc_entities);
}

std::vector<EntityId> ex1_map1_entites(
        {EntityId("Pnt1"), EntityId("Ju01"), EntityId("Ju02"), EntityId("Ju04"), EntityId("Ju03"), EntityId("West1"),
         EntityId("East1")});
std::vector<EntityId> ex1_map2_entites(
        {EntityId("P1"), EntityId("J1"), EntityId("J2"), EntityId("J3"), EntityId("J4"), EntityId("W1"),
         EntityId("W2")});

TEST(GISEx1, loadMapFileMap1) {
    GIS gis = GISProvider::getGIS();
    std::vector<EntityId> loaded_entities = gis.loadMapFile("ex1_map1.json");
    EXPECT_EQ(loaded_entities, ex1_map1_entites);
}

TEST(GISEx1, loadMapFileMap1NoIds) {
    GIS gis = GISProvider::getGIS();
    std::vector<EntityId> loaded_entities = gis.loadMapFile("ex1_map1.no_ids.json");
    std::size_t number_of_loaded_entities = loaded_entities.size();
    std::size_t expected_number_of_loaded_entities = 5;
    EXPECT_EQ(number_of_loaded_entities, expected_number_of_loaded_entities);
}

TEST(GISEx1, loadMapFileMap1MixedIds) {
    GIS gis = GISProvider::getGIS();
    std::vector<EntityId> loaded_entities = gis.loadMapFile("ex1_map1.mixed_ids_file.json");
    std::size_t number_of_loaded_entities = loaded_entities.size();
    std::set<std::size_t> expected_number_of_loaded_entities({1, 6, 7});
    EXPECT_TRUE(expected_number_of_loaded_entities.count(number_of_loaded_entities));
}

TEST(GISEx1, loadMapFileMap2) {
    GIS gis = GISProvider::getGIS();
    std::vector<EntityId> loaded_entities = gis.loadMapFile("ex1_map2.json");
    EXPECT_EQ(loaded_entities, ex1_map2_entites);
}

TEST(GISEx1, twoLoadsSameFile) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map1.json");
    gis.loadMapFile("ex1_map1.json");
    std::vector<EntityId> entities = gis.getEntities("LWS");
    std::vector<EntityId> expected_entities({EntityId("Ju02")});
    EXPECT_EQ(entities, expected_entities);
}

TEST(GISEx1, twoLoadsMap1Map2) {
    GIS gis = GISProvider::getGIS();
    std::vector<EntityId> loaded_entities1 = gis.loadMapFile("ex1_map1.json");
    std::vector<EntityId> expected_entities1(
            {EntityId("Pnt1"), EntityId("Ju01"), EntityId("Ju02"), EntityId("Ju04"), EntityId("Ju03"),
             EntityId("West1"), EntityId("East1")});
    EXPECT_EQ(loaded_entities1, expected_entities1);
    std::vector<EntityId> loaded_entities2 = gis.loadMapFile("ex1_map2.json");
    std::vector<EntityId> expected_entities2(
            {EntityId("P1"), EntityId("J1"), EntityId("J2"), EntityId("J3"), EntityId("J4"), EntityId("W1"),
             EntityId("W2")});
    EXPECT_EQ(loaded_entities2, expected_entities2);
}

TEST(GISEx1, twoLoadsMap1Map2AndGetEntities) {
    GIS gis = GISProvider::getGIS();
    std::vector<EntityId> loaded_entities1 = gis.loadMapFile("ex1_map1.json");
    std::vector<EntityId> expected_entities1(
            {EntityId("Pnt1"), EntityId("Ju01"), EntityId("Ju02"), EntityId("Ju04"), EntityId("Ju03"),
             EntityId("West1"), EntityId("East1")});
    EXPECT_EQ(loaded_entities1, expected_entities1);
    std::vector<EntityId> loaded_entities2 = gis.loadMapFile("ex1_map2.json");
    std::vector<EntityId> expected_entities2(
            {EntityId("P1"), EntityId("J1"), EntityId("J2"), EntityId("J3"), EntityId("J4"), EntityId("W1"),
             EntityId("W2")});
    EXPECT_EQ(loaded_entities2, expected_entities2);
    std::vector<EntityId> entities3 = gis.getEntities("LWS");
    std::vector<EntityId> expected_entities3({EntityId("J2"), EntityId("Ju02")});
    std::sort(entities3.begin(), entities3.end());
    EXPECT_EQ(entities3, expected_entities3);
}

constexpr double ex1_epsilon = 1;

template<typename LL>
//TODO: Use LatLong in ex2
bool testLatLongEqualityInEx1(LL tested, LL golden) {
    return std::fabs(static_cast<double>(tested) - static_cast<double>(golden)) < ex1_epsilon;
}

bool testCoordinatsEqualityInEx1(const Coordinates tested, const Coordinates &golden) {
    return testLatLongEqualityInEx1(tested.longitude(), golden.longitude()) &&
           testLatLongEqualityInEx1(tested.latitude(), golden.latitude());
}

TEST(GISEx1, getEntityClosestPointNotKnown) {
// [1] If the Id is not known returns an empty optional
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map1.json");
    Coordinates point{Longitude{40.0}, Latitude{10.0}};
    Coordinates pnt_center{Longitude{20.0}, Latitude{70.0}};
    Meters pnt_radius{1001.96};
    std::optional<Coordinates> closest_point = gis.getEntityClosestPoint(EntityId{"non-exixtant"}, point);
    ASSERT_FALSE(closest_point);
}

TEST(GISEx1, getEntityClosestPoint1) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map1.json");
    Coordinates point{Longitude{40.0}, Latitude{10.0}};
    Coordinates pnt_center{Longitude{20.0}, Latitude{70.0}};
    Meters pnt_radius{1001.96};
    std::optional<Coordinates> closest_point = gis.getEntityClosestPoint(EntityId{"Pnt1"}, point);
    Coordinates expected_point{Longitude{20.0101094228863}, Latitude{69.9916792225197}};
    ASSERT_TRUE(closest_point);
    EXPECT_TRUE(testCoordinatsEqualityInEx1(*closest_point, expected_point));
}

TEST(GISEx1, getEntityClosestPoint2) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map2.json");
    Coordinates point{Longitude{-1}, Latitude{-1}};
    Coordinates pnt_center{Longitude{20.730811}, Latitude{10.997455}};
    Meters pnt_radius{100.96};
    std::optional<Coordinates> closest_point = gis.getEntityClosestPoint(EntityId{"P1"}, point);
    Coordinates expected_point{Longitude{20.7299920271604}, Latitude{10.9970330126177}};
    ASSERT_TRUE(closest_point);
    EXPECT_TRUE(testCoordinatsEqualityInEx1(*closest_point, expected_point));
}

TEST(GISEx2, getEntityClosestPoint1) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map1.json");
    Coordinates point{Longitude{40.0}, Latitude{10.0}};
    Coordinates pnt_center{Longitude{20.0}, Latitude{70.0}};
    Meters pnt_radius{1001.96};
    std::optional<Coordinates> closest_point = gis.getEntityClosestPoint(EntityId{"Pnt1"}, point);
    Coordinates expected_point = CoordinatesMath::closestPointOnCircle(point, pnt_center, pnt_radius);
    ASSERT_TRUE(closest_point);
    EXPECT_TRUE(testCoordinatsEqualityInEx1(*closest_point, expected_point));
}

TEST(GISEx2, getEntityClosestPoint2) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map2.json");
    Coordinates point{Longitude{-1}, Latitude{-1}};
    Coordinates pnt_center{Longitude{20.730811}, Latitude{10.997455}};
    Meters pnt_radius{100.96};
    std::optional<Coordinates> closest_point = gis.getEntityClosestPoint(EntityId{"P1"}, point);
    Coordinates expected_point = CoordinatesMath::closestPointOnCircle(point, pnt_center, pnt_radius);
    ASSERT_TRUE(closest_point);
    EXPECT_TRUE(testCoordinatsEqualityInEx1(*closest_point, expected_point));
}

TEST(GISEx1, getWayClosestPoint) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map1.json");
    Coordinates east_point{Longitude{15}, Latitude{-50}};
    std::pair<Coordinates, EntityId> closest = gis.getWayClosestPoint(east_point);
    EXPECT_EQ(closest.second, EntityId{"East1"});
    Coordinates ju_04{Longitude{20}, Latitude{-20.0}};
    EXPECT_EQ(closest.first, ju_04);
}

TEST(GISEx1, getWayClosestPointNorth) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map2.json");
    Coordinates northern_point{Longitude{-1}, Latitude{60}};
    std::pair<Coordinates, EntityId> closest_from_gis = gis.getWayClosestPoint(northern_point);
    EXPECT_EQ(closest_from_gis.second, EntityId{"W1"});
    Coordinates closest_to_northern_way{Longitude{-3.55086}, Latitude{81.3014}};
    EXPECT_TRUE(testCoordinatsEqualityInEx1(closest_from_gis.first, closest_to_northern_way));
}

TEST(GISEx1, getWayClosestPointSouth) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map2.json");
    Coordinates southern_point{Longitude{21}, Latitude{-1}};
    std::pair<Coordinates, EntityId> closest_from_gis = gis.getWayClosestPoint(southern_point);
    Coordinates closest_to_southern_way{Longitude{21.7155}, Latitude{-10.7113}};
    EXPECT_TRUE(testCoordinatsEqualityInEx1(closest_from_gis.first, closest_to_southern_way));
}

TEST(GISEx1, getEntities) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map1.json");
    std::vector<EntityId> entities = gis.getEntities("The_great_Highway");
    std::vector<EntityId> expected_entities({EntityId("East1"), EntityId("West1")});
    std::sort(entities.begin(), entities.end());
    EXPECT_EQ(entities, expected_entities);
}

TEST(GISEx1, getEntitiesRadius) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map1.json");
    std::vector<EntityId> entities = gis.getEntities("The_great_Highway", {Longitude{-20}, Latitude{-9.67}},
                                                     Meters{50'000});
    std::vector<EntityId> expected_entities({EntityId("West1")});
    EXPECT_EQ(entities, expected_entities);
}


