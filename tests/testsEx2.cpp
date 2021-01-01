//
// Created by student on 23/12/2020.
//

#include <cstdio>
#include "gtest/gtest.h"

// Ex1
#include "../GISProvider.h"
#include "../Common/CoordinatesMath.h"

// Ex2
#include "../Common/NavigationGIS.h"
#include "../Common/GISNamedTypes.h"
#include "../navigation/Navigation.h"
#include "../navigation/Routes.h"
#include "../navigation/Route.h"
#include "NavigationValidator.h"

/**
 * @brief Returns input file entites in order defined
 *
 * @return std::vector<EntityId>
 */
const std::vector<EntityId> &getInputFileEntities(const std::string &filename) {
    if (filename == "nyc.json") {
        static std::vector<EntityId> nyc_entities(
                {EntityId("P1001"), EntityId("P1002"), EntityId("J1001"), EntityId("J1002"), EntityId("J1010"),
                 EntityId("P1010"), EntityId("W2001"), EntityId("W2002")});
        return nyc_entities;
    }
    if (filename == "ex1_map1.json") {
        static std::vector<EntityId> ex1_map1_entites(
                {EntityId("Pnt1"), EntityId("Ju01"), EntityId("Ju02"), EntityId("Ju04"), EntityId("Ju03"),
                 EntityId("West1"), EntityId("East1")});
        return ex1_map1_entites;
    }
    if (filename == "ex1_map2.json") {
        static std::vector<EntityId> ex1_map2_entites(
                {EntityId("P1"), EntityId("J1"), EntityId("J2"), EntityId("J3"), EntityId("J4"), EntityId("W1"),
                 EntityId("W2")});
        return ex1_map2_entites;
    }
    if (filename == "ex2_map1.json") {
        static std::vector<EntityId> ex2_map1_entites(
                {EntityId("P1"), EntityId("J1"), EntityId("J2"), EntityId("J3"), EntityId("J4"), EntityId("J5"),
                 EntityId("P2"), EntityId("W1"), EntityId("W2"), EntityId("W3"), EntityId("W4"), EntityId("W5")});
        return ex2_map1_entites;
    }
    //Empty file
    static std::vector<EntityId> empty_vector{};
    return empty_vector;
}

/**
 * @brief Get Entities object of both files in undefined order - eliminates duplicates
 *
 * @param filename1
 * @param filename1
 * @return std::vector<EntityId>
 */
std::vector<EntityId> getCombinedFilesEntities(const std::string &filename1, const std::string &filename2) {
    auto map1_entities = getInputFileEntities(filename1);
    auto map2_entities = getInputFileEntities(filename2);
    std::vector<EntityId> merged_entities;
    merged_entities.reserve(map1_entities.size() + map2_entities.size()); // preallocate memory
    merged_entities.insert(merged_entities.end(), map1_entities.begin(), map1_entities.end());
    merged_entities.insert(merged_entities.end(), map2_entities.begin(), map2_entities.end());
    // Clear duplicates
    std::sort(merged_entities.begin(), merged_entities.end());
    merged_entities.erase(std::unique(merged_entities.begin(), merged_entities.end()), merged_entities.end());
    return merged_entities;
}

TEST(GISPre, loadMapFile) {
    GIS gis = GISProvider::getGIS();
    std::string input_file{"nyc.json"};
    std::vector<EntityId> loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));
}

TEST(GISPre, getEntities) {
    GIS gis = GISProvider::getGIS();
    std::string input_file{"nyc.json"};
    gis.loadMapFile(input_file);
    std::vector<EntityId> entities = gis.getEntities("Washington Square Park");
    std::sort(entities.begin(), entities.end());
    std::vector<EntityId> expected_entities({EntityId("P1001")});
    EXPECT_EQ(entities, expected_entities);
}

TEST(GISPRe, clearReturnValue) {
    GIS gis = GISProvider::getGIS();
    std::string input_file{"nyc.json"};
    gis.loadMapFile(input_file);
    std::size_t number_of_cleared_entities = gis.clear();
    EXPECT_EQ(number_of_cleared_entities, getInputFileEntities(input_file).size());
}

TEST(GISPre, saveMapFileReturnValue) {
    GIS gis = GISProvider::getGIS();
    std::string input_file{"nyc.json"};
    std::string output_file{"out1." + input_file};
    gis.loadMapFile(input_file);
    std::size_t number_of_saved_entities = gis.saveMapFile(output_file);
    EXPECT_EQ(number_of_saved_entities, getInputFileEntities(input_file).size());
    std::remove(output_file.c_str());
}

TEST(GISEx1, saveMapFileLoadSavedMap) {
    GIS gis = GISProvider::getGIS();
    std::string input_file{"nyc.json"};
    gis.loadMapFile(input_file);
    std::string output_file{"out2." + input_file};
    std::size_t number_of_saved_entities = gis.saveMapFile(output_file);
    EXPECT_EQ(number_of_saved_entities, getInputFileEntities(input_file).size());
    gis.clear();
    auto loaded_entities = gis.loadMapFile(input_file);
    std::sort(loaded_entities.begin(), loaded_entities.end());
    std::vector<EntityId> expected_entites = getInputFileEntities(input_file);
    std::sort(expected_entites.begin(), expected_entites.end());
    EXPECT_EQ(loaded_entities, expected_entites);
    std::remove(output_file.c_str());
}

TEST(GISEx1, clearLoad) {
    GIS gis = GISProvider::getGIS();
    std::string input_file{"nyc.json"};
    gis.loadMapFile(input_file);
    gis.clear();
    std::vector<EntityId> loaded_entities = gis.loadMapFile("nyc.json");
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));
}

TEST(GISEx1, loadMapFileMap1) {
    GIS gis = GISProvider::getGIS();
    std::string input_file{"ex1_map1.json"};
    std::vector<EntityId> loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));
}

TEST(GISEx1, loadMapFileMap2) {
    GIS gis = GISProvider::getGIS();
    std::string input_file{"ex1_map2.json"};
    std::vector<EntityId> loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));
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
    std::string input_file1{"ex1_map1.json"};
    std::vector<EntityId> loaded_entities1 = gis.loadMapFile(input_file1);
    EXPECT_EQ(loaded_entities1, getInputFileEntities(input_file1));
    std::string input_file2{"ex1_map2.json"};
    std::vector<EntityId> loaded_entities2 = gis.loadMapFile(input_file2);
    EXPECT_EQ(loaded_entities2, getInputFileEntities(input_file2));
}

TEST(GISEx1, twoLoadsMap1Map2AndGetEntities) {
    GIS gis = GISProvider::getGIS();
    std::string input_file1{"ex1_map1.json"};
    gis.loadMapFile(input_file1);
    std::string input_file2{"ex1_map2.json"};
    gis.loadMapFile(input_file2);
    std::vector<EntityId> entities = gis.getEntities("LWS");
    std::vector<EntityId> expected_entities({EntityId("J2"), EntityId("Ju02")});
    std::sort(entities.begin(), entities.end());
    EXPECT_EQ(entities, expected_entities);
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
    auto closest = gis.getWayClosestPoint(east_point);
    EXPECT_EQ(std::get<1>(closest), EntityId{"East1"});
    Coordinates ju_04{Longitude{20}, Latitude{-20.0}};
    EXPECT_EQ(std::get<0>(closest), ju_04);
}

TEST(GISEx1, getWayClosestPointNorth) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map2.json");
    Coordinates northern_point{Longitude{-1}, Latitude{60}};
    auto closest_from_gis = gis.getWayClosestPoint(northern_point);
    EXPECT_EQ(std::get<1>(closest_from_gis), EntityId{"W1"});
    Coordinates closest_to_northern_way{Longitude{-3.55086}, Latitude{81.3014}};
    EXPECT_TRUE(testCoordinatsEqualityInEx1(std::get<0>(closest_from_gis), closest_to_northern_way));
}

TEST(GISEx1, getWayClosestPointSouth) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map2.json");
    Coordinates southern_point{Longitude{21}, Latitude{-1}};
    auto closest_from_gis = gis.getWayClosestPoint(southern_point);
    Coordinates closest_to_southern_way{Longitude{21.7155}, Latitude{-10.7113}};
    EXPECT_TRUE(testCoordinatsEqualityInEx1(std::get<0>(closest_from_gis), closest_to_southern_way));
}

TEST(GISEx1, getEntities) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map2.json");
    std::vector<EntityId> entities = gis.getEntities("A_way");
    std::vector<EntityId> expected_entities({EntityId("W1"), EntityId("W2")});
    std::sort(entities.begin(), entities.end());
    EXPECT_EQ(entities, expected_entities);
}

namespace gis_testing {
    struct Way {
        Coordinates from;
        Coordinates to;
    };
}

gis_testing::Way ex1_map2_W2{{Longitude{-30}, Latitude{-10}},
                             {Longitude{30},  Latitude{-10}}};

TEST(GISEx1, getEntitiesRadius) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("ex1_map2.json");
    Coordinates point{Longitude{-20}, Latitude{-9.94}};
    Meters circle_radius{100'000};
    std::vector<EntityId> entities = gis.getEntities("A_way", point, circle_radius);
    auto distance_from_way = CoordinatesMath::distanceFromSegment(point, ex1_map2_W2.from, ex1_map2_W2.to);
    EXPECT_TRUE(distance_from_way <= circle_radius); // Sanity check
    std::vector<EntityId> expected_entities({EntityId("W2")});
    EXPECT_EQ(entities, expected_entities);
}

TEST(GISEx1, loadMap1SaveClearAndLoad) {
    GIS gis = GISProvider::getGIS();
    std::string input_file{"ex1_map1.json"};
    auto loaded_input = gis.loadMapFile(input_file);
    std::sort(loaded_input.begin(), loaded_input.end());
    std::string output_file{"out." + input_file};
    std::size_t saved = gis.saveMapFile(output_file);
    std::size_t number_of_cleared_entities = gis.clear();
    EXPECT_TRUE(number_of_cleared_entities == saved && saved == getInputFileEntities(input_file).size());
    auto loaded_output = gis.loadMapFile(output_file);
    std::sort(loaded_output.begin(), loaded_output.end());
    EXPECT_EQ(loaded_input, loaded_output);
    std::remove(output_file.c_str());
}

TEST(GISEx1, loadMap2SaveClearAndLoad) {
    GIS gis = GISProvider::getGIS();
    std::string input_file{"ex1_map2.json"};
    auto loaded_input = gis.loadMapFile(input_file);
    std::sort(loaded_input.begin(), loaded_input.end());
    std::string output_file{"out." + input_file};
    std::size_t saved = gis.saveMapFile(output_file);
    std::size_t number_of_cleared_entities = gis.clear();
    EXPECT_TRUE(number_of_cleared_entities == saved && saved == getInputFileEntities(input_file).size());
    auto loaded_output = gis.loadMapFile(output_file);
    std::sort(loaded_output.begin(), loaded_output.end());
    EXPECT_EQ(loaded_input, loaded_output);
    std::remove(output_file.c_str());
}

TEST(GISEx1, twoLoadsMap1Map2ClearSaveAndLoad) {
    GIS gis = GISProvider::getGIS();
    std::string input_file1{"ex1_map1.json"};
    std::vector<EntityId> loaded_entities1 = gis.loadMapFile(input_file1);
    std::string input_file2{"ex1_map2.json"};
    std::vector<EntityId> loaded_entities2 = gis.loadMapFile(input_file2);
    std::string output_file{"out_ex1_map1map2.json"};
    std::size_t number_of_saved_entities = gis.saveMapFile(output_file);
    std::size_t number_of_cleared_entities = gis.clear();
    EXPECT_TRUE(getInputFileEntities(input_file1).size() + getInputFileEntities(input_file2).size() ==
                number_of_saved_entities
                && number_of_saved_entities == number_of_cleared_entities);
    std::vector<EntityId> loaded_entities = gis.loadMapFile(output_file);
    std::vector<EntityId> combined_entities = getCombinedFilesEntities(input_file1, input_file2);
    std::sort(combined_entities.begin(), combined_entities.end());
    std::sort(loaded_entities.begin(), loaded_entities.end());
    EXPECT_EQ(loaded_entities, combined_entities);
    std::remove(output_file.c_str());
}

TEST(GISEx1, loadNonExistantFile) {
    GIS gis = GISProvider::getGIS();
    std::string input_file{"___non_existant_file___"};
    std::vector<EntityId> loaded_entities = gis.loadMapFile(input_file);
    std::size_t expected_number_of_loaded_entities = 0;
    EXPECT_EQ(loaded_entities.size(), expected_number_of_loaded_entities);
}

// Testing Ex2 Functionality

Coordinates ex2_map1_P1{Longitude{-30.24}, Latitude{50}};
Coordinates ex2_map1_P2{Longitude{-25.282}, Latitude{48.12}};

Coordinates ex2_map1_J1{Longitude{-30.23}, Latitude{49.84}};
Coordinates ex2_map1_J2{Longitude{-29.76}, Latitude{49.12}};
Coordinates ex2_map1_J3{Longitude{-29.02}, Latitude{51}};
Coordinates ex2_map1_J4{Longitude{-27.3}, Latitude{48.2}};
Coordinates ex2_map1_J5{Longitude{-25.31}, Latitude{48.53}};

gis_testing::Way ex2_map1_W1{ex2_map1_J1, ex2_map1_J2};
gis_testing::Way ex2_map1_W2{ex2_map1_J2, ex2_map1_J3};
gis_testing::Way ex2_map1_W3{ex2_map1_J2, ex2_map1_J4};
gis_testing::Way ex2_map1_W4{ex2_map1_J3, ex2_map1_J4};
gis_testing::Way ex2_map1_W5{ex2_map1_J4, ex2_map1_J5};

std::vector<std::pair<EntityId, Direction>> ex2_map1_best_route_ways{std::make_pair(EntityId{"W1"}, Direction::A_to_B),
                                                                     std::make_pair(EntityId{"W3"}, Direction::A_to_B),
                                                                     std::make_pair(EntityId{"W5"}, Direction::A_to_B)};

TEST(GISEx2, NavigationTest) {
    GIS gis = GISProvider::getGIS();
    std::string input_file{"ex2_map1.json"};
    auto loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));

    NavigationGIS navigation_gis{gis};
    Navigation navigation{navigation_gis};

    Coordinates start{ex2_map1_P1}; // Note: this point is not on a way (could be on way in other cases)
    Coordinates end{ex2_map1_P2}; // Note: this point is not on a way (could be on way in other cases)
    Routes routes = navigation.getRoutes(start, end);
    EXPECT_TRUE(routes.isValid());

    const Route &shortest_distance_route = routes.shortestDistance();
    const Route &shortest_time_route = routes.shortestTime();

    EXPECT_EQ(shortest_distance_route.getWays(), ex2_map1_best_route_ways);
    EXPECT_EQ(shortest_time_route.getWays(), ex2_map1_best_route_ways);

    Coordinates expected_route_start_point = CoordinatesMath::closestPointOnSegment(start, ex2_map1_W1.from,
                                                                                    ex2_map1_W1.to);
    Coordinates expected_route_end_point = CoordinatesMath::closestPointOnSegment(end, ex2_map1_W5.from,
                                                                                  ex2_map1_W5.to);

    // Note: route start point is on W1 - different from start
    // A start point of Route is always on a way!
    EXPECT_EQ(shortest_distance_route.getWayStartPoint(), expected_route_start_point);
    EXPECT_EQ(shortest_time_route.getWayStartPoint(), expected_route_start_point);

    // Note: route end point is on W5 - different from end
    // A end point of Route is always on a way!
    EXPECT_EQ(shortest_distance_route.getWayEndPoint(), expected_route_end_point);
    EXPECT_EQ(shortest_time_route.getWayEndPoint(), expected_route_end_point);

    NavigationValidator navigation_validator{gis};

    // Note: we are validating the route with start and end point that are not on ways (could be on way in other cases)
    EXPECT_TRUE(navigation_validator.validateRoute(start, end, shortest_distance_route));
    EXPECT_TRUE(navigation_validator.validateRoute(start, end, shortest_time_route));
}

