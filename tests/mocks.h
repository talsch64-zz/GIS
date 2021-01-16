#pragma once 

#include "../Common/GISNamedTypes.h"
#include <string> 
#include <vector>

/**
 * @brief Returns input file entites in order defined
 * 
 * @return std::vector<EntityId> 
 */
const std::vector<EntityId>& getInputFileEntities(const std::string& filename) {
	if (filename == "nyc.json") {
		static std::vector<EntityId> nyc_entities({EntityId("P1001"), EntityId("P1002"), EntityId("J1001"), EntityId("J1002"), EntityId("J1010"), EntityId("P1010"), EntityId("W2001"), EntityId("W2002")});	
		return nyc_entities;
	}
	if (filename == "ex1_map1.json") {
		static std::vector<EntityId> ex1_map1_entites({EntityId("Pnt1"), EntityId("Ju01"), EntityId("Ju02"), EntityId("Ju04"), EntityId("Ju03"), EntityId("West1"), EntityId("East1")});
		return ex1_map1_entites;
	}
	if (filename == "ex1_map2.json") {
		static std::vector<EntityId> ex1_map2_entites({EntityId("P1"), EntityId("J1"), EntityId("J2"), EntityId("J3"), EntityId("J4"), EntityId("W1"), EntityId("W2")});	
		return ex1_map2_entites; 
	} 
	if (filename == "ex1_two_ways.json" || filename == "ex1_two_ways_180th_meridian.json") {
		static std::vector<EntityId> ex1_two_ways({EntityId("TL"), EntityId("BL"), EntityId("LeftWay"), EntityId("TR"), EntityId("BR"), EntityId("RightWay")});	
		return ex1_two_ways; 
	} 
	if (filename == "ex2_map1.json") {
		static std::vector<EntityId> ex2_map1_entites({EntityId("P1"), EntityId("J1"), EntityId("J2"), EntityId("J3"), EntityId("J4"), EntityId("J5"), EntityId("P2"), EntityId("W1"), EntityId("W2"), EntityId("W3"), EntityId("W4"), EntityId("W5")});	
		return ex2_map1_entites; 
	}
        if (filename == "ex2_map2.json") {
                static std::vector<EntityId> ex2_map2_entites({EntityId("P1"), EntityId("J1"), EntityId("J2"), EntityId("J3"), EntityId("J4"), EntityId("J5"), EntityId("P2"), EntityId("W1"), EntityId("W2"), EntityId("W3"), EntityId("W4"), EntityId("W5")});
                return ex2_map2_entites;
        }
	if (filename == "ex2_map2_highways.json") {
                static std::vector<EntityId> ex2_map2_highways_entites({EntityId("P1"), EntityId("J1"), EntityId("J2"), EntityId("J3"), EntityId("J4"), EntityId("J5"), EntityId("P2"), EntityId("W1"), EntityId("W2"), EntityId("W3"), EntityId("W4"), EntityId("W5")});
                return ex2_map2_highways_entites;
        }
        if (filename == "ex2_map3_uniderectional_edges.json") {
                static std::vector<EntityId> ex2_map3_uniderectional_edges_entites({EntityId("P1"), EntityId("J1"), EntityId("J2"), EntityId("J3"), EntityId("J4"), EntityId("J5"), EntityId("P2"), EntityId("W1"), EntityId("W2"), EntityId("W3"), EntityId("W4"), EntityId("W5")});
                return ex2_map3_uniderectional_edges_entites;
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
std::vector<EntityId> getCombinedFilesEntities(const std::string& filename1, const std::string& filename2) {
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

namespace gis_testing {
	struct Way {
		Coordinates from; 
		Coordinates to; 
	};
}

/**
 * @brief ex1_map2.json
 * 
 */
gis_testing::Way ex1_map2_W2{{Longitude{-30}, Latitude{-10}}, {Longitude{30}, Latitude{-10}}};

/**
 * @brief ex1_two_ways
 * 
 */
Coordinates ex1_two_ways_TL{Longitude{44.361}, Latitude{33.312}};
Coordinates ex1_two_ways_BL{Longitude{44.361}, Latitude{27.312}};
Coordinates ex1_two_ways_TR{Longitude{50.361}, Latitude{33.312}};
Coordinates ex1_two_ways_BR{Longitude{50.361}, Latitude{27.312}};

gis_testing::Way ex1_two_ways_LeftWay{ex1_two_ways_BL, ex1_two_ways_TL};
gis_testing::Way ex1_two_ways_RightWay{ex1_two_ways_BR, ex1_two_ways_TR};

/**
 * @brief ex1_two_ways_180th_meridan
 * 
 */
Coordinates ex1_two_ways_180th_meridan_TL{Longitude{44.361}, Latitude{33.312}};
Coordinates ex1_two_ways_180th_meridan_BL{Longitude{44.361}, Latitude{27.312}};
Coordinates ex1_two_ways_180th_meridan_TR{Longitude{50.361}, Latitude{33.312}};
Coordinates ex1_two_ways_180th_meridan_BR{Longitude{50.361}, Latitude{27.312}};

gis_testing::Way ex1_two_ways_180th_meridan_LeftWay{ex1_two_ways_180th_meridan_BL, ex1_two_ways_180th_meridan_TL};
gis_testing::Way ex1_two_ways_180th_meridan_RightWay{ex1_two_ways_180th_meridan_BR, ex1_two_ways_180th_meridan_TR};

/**
 * @brief ex2_map1.json
 *
 */
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

std::vector<std::pair<EntityId, Direction>> ex2_map1_best_route_ways{std::make_pair(EntityId{"W1"}, Direction::A_to_B), std::make_pair(EntityId{"W3"}, Direction::A_to_B), std::make_pair(EntityId{"W5"}, Direction::A_to_B)};

/**
 * @brief ex2_map2.json
 *
 */
Coordinates ex2_map2_P1{Longitude{-30.24}, Latitude{50}};
Coordinates ex2_map2_P2{Longitude{-25.282}, Latitude{48.12}};

Coordinates ex2_map2_J1{Longitude{-30.23}, Latitude{49.84}};
Coordinates ex2_map2_J2{Longitude{-29.76}, Latitude{49.12}};
Coordinates ex2_map2_J3{Longitude{-29.02}, Latitude{51}};
Coordinates ex2_map2_J4{Longitude{-27.3}, Latitude{48.2}};
Coordinates ex2_map2_J5{Longitude{-25.31}, Latitude{48.53}};

gis_testing::Way ex2_map2_W1{ex2_map2_J1, ex2_map2_J2};
gis_testing::Way ex2_map2_W2{ex2_map2_J2, ex2_map2_J3};
gis_testing::Way ex2_map2_W3{ex2_map2_J2, ex2_map2_J4};
gis_testing::Way ex2_map2_W4{ex2_map2_J3, ex2_map2_J4};
gis_testing::Way ex2_map2_W5{ex2_map2_J4, ex2_map2_J5};

std::vector<std::pair<EntityId, Direction>> ex2_map2_shortest_route{std::make_pair(EntityId{"W1"}, Direction::A_to_B), std::make_pair(EntityId{"W3"}, Direction::A_to_B), std::make_pair(EntityId{"W5"}, Direction::A_to_B)};
std::vector<std::pair<EntityId, Direction>> ex2_map2_fastest_route{std::make_pair(EntityId{"W1"}, Direction::A_to_B), std::make_pair(EntityId{"W2"}, Direction::A_to_B), std::make_pair(EntityId{"W4"}, Direction::A_to_B), std::make_pair(EntityId{"W5"}, Direction::A_to_B)};

/**
 * @brief ex2_map2_highways.json
 *
 */
std::vector<std::pair<EntityId, Direction>> ex2_map2_highways_no_highways_route{std::make_pair(EntityId{"W2"}, Direction::B_to_A), std::make_pair(EntityId{"W3"}, Direction::A_to_B)};

/**
 * @brief ex2_map4_curves2.json
 *
 */

Coordinates ex2_map4_J6{Longitude{-28.3}, Latitude{47.1}};

std::vector<std::pair<EntityId, Direction>> ex2_map4_curves2_fastest_route{std::make_pair(EntityId{"W1"}, Direction::A_to_B), std::make_pair(EntityId{"W2"}, Direction::A_to_B), std::make_pair(EntityId{"W6"}, Direction::A_to_B), std::make_pair(EntityId{"W7"}, Direction::A_to_B), std::make_pair(EntityId{"W5"}, Direction::A_to_B)};

std::vector<std::pair<EntityId, Direction>> ex2_map4_curves2_shortest_route{std::make_pair(EntityId{"W1"}, Direction::A_to_B), std::make_pair(EntityId{"W3"}, Direction::A_to_B), std::make_pair(EntityId{"W5"}, Direction::A_to_B)};

