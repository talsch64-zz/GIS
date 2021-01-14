#include <cstdio>
#include <fstream>
#include "gtest/gtest.h"

#include "mocks.h"

// Ex1
#include "../GIS/GISProvider.h"
#include "../Common/CoordinatesMath.h"

// Ex2
#include "../Simulator/NavigationValidator.h"
#include "../Navigation/NavigationProvider.h"
#include "../Common/NavigationGIS.h"

// Testing Ex2 Functionality

TEST(GISEx2, NavigationTest) {
    auto gis = GISProvider::getGIS();
    std::string input_file{"ex2_map1.json"};
    auto loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));

    NavigationGIS navigation_gis{gis};
    auto navigation = NavigationProvider::getNavigation(navigation_gis);

    Coordinates start{ex2_map1_P1}; // Note: this point is not on a way (could be on way in other cases)
    Coordinates end{ex2_map1_P2}; // Note: this point is not on a way (could be on way in other cases)
    auto routes = navigation.getRoutes(start, end);
    EXPECT_TRUE(routes->isValid());

    const AbstractRoute& shortest_distance_route = routes->shortestDistance();
    const AbstractRoute& shortest_time_route = routes->shortestTime();

    EXPECT_EQ(shortest_distance_route.getWays(), ex2_map1_best_route_ways);
    EXPECT_EQ(shortest_time_route.getWays(), ex2_map1_best_route_ways);

    Coordinates expected_route_start_point = CoordinatesMath::closestPointOnSegment(start, ex2_map1_W1.from, ex2_map1_W1.to);
    Coordinates expected_route_end_point = CoordinatesMath::closestPointOnSegment(end, ex2_map1_W5.from, ex2_map1_W5.to);

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


std::size_t numberOfLines(const char* filename) {
    std::string line;
    std::size_t number_of_lines = 0;
    std::ifstream myfile(filename);
    while (std::getline(myfile, line))
        ++number_of_lines;
    return number_of_lines;
}

TEST(GISEx2, CreateErrorsLog) {
    const char* errors_logname = "errors.log";
    std::remove(errors_logname);
    auto gis = GISProvider::getGIS();
    std::string input_file{"ex2_map1.json"};
    auto loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));
    auto loaded_entities2 = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities2.size(), (std::size_t) 0);
    EXPECT_TRUE(numberOfLines(errors_logname) >= loaded_entities.size());
}

TEST(GISEx2, NavigationTestFastestDifferentFromShortest) {
    auto gis = GISProvider::getGIS();
    std::string input_file{"ex2_map2.json"};
    auto loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));

    NavigationGIS navigation_gis{gis};
    auto navigation = NavigationProvider::getNavigation(navigation_gis);

    Coordinates start{ex2_map2_P1}; // Note: this point is not on a way (could be on way in other cases)
    Coordinates end{ex2_map2_P2}; // Note: this point is not on a way (could be on way in other cases)
    auto routes = navigation.getRoutes(start, end);
    EXPECT_TRUE(routes->isValid());

    const AbstractRoute& shortest_distance_route = routes->shortestDistance();
    const AbstractRoute& shortest_time_route = routes->shortestTime();

    EXPECT_EQ(shortest_distance_route.getWays(), ex2_map2_shortest_route);
    EXPECT_EQ(shortest_time_route.getWays(), ex2_map2_fastest_route);

    Coordinates expected_route_start_point = CoordinatesMath::closestPointOnSegment(start, ex2_map2_W1.from, ex2_map2_W1.to);
    Coordinates expected_route_end_point = CoordinatesMath::closestPointOnSegment(end, ex2_map2_W5.from, ex2_map2_W5.to);

    EXPECT_EQ(shortest_distance_route.getWayStartPoint(), expected_route_start_point);
    EXPECT_EQ(shortest_time_route.getWayStartPoint(), expected_route_start_point);

    EXPECT_EQ(shortest_distance_route.getWayEndPoint(), expected_route_end_point);
    EXPECT_EQ(shortest_time_route.getWayEndPoint(), expected_route_end_point);

    NavigationValidator navigation_validator{gis};

    EXPECT_TRUE(navigation_validator.validateRoute(start, end, shortest_distance_route));
    EXPECT_TRUE(navigation_validator.validateRoute(start, end, shortest_time_route));
}

TEST(GISEx2, NavigationTestCurves1) {
    auto gis = GISProvider::getGIS();
    std::string input_file{"ex2_map2_curves1.json"};
    gis.loadMapFile(input_file);

    NavigationGIS navigation_gis{gis};
    auto navigation = NavigationProvider::getNavigation(navigation_gis);

    Coordinates start{ex2_map2_P1}; // Note: this point is not on a way (could be on way in other cases)
    Coordinates end{ex2_map2_P2}; // Note: this point is not on a way (could be on way in other cases)
    auto routes = navigation.getRoutes(start, end);
    EXPECT_TRUE(routes->isValid());

    const AbstractRoute& shortest_distance_route = routes->shortestDistance();
    const AbstractRoute& shortest_time_route = routes->shortestTime();

    EXPECT_EQ(shortest_distance_route.getWays(), ex2_map2_fastest_route);
    EXPECT_EQ(shortest_time_route.getWays(), ex2_map2_fastest_route);

    Coordinates expected_route_start_point = CoordinatesMath::closestPointOnSegment(start, ex2_map2_W1.from, ex2_map2_W1.to);
    Coordinates expected_route_end_point = CoordinatesMath::closestPointOnSegment(end, ex2_map2_W5.from, ex2_map2_W5.to);

    EXPECT_EQ(shortest_distance_route.getWayStartPoint(), expected_route_start_point);
    EXPECT_EQ(shortest_time_route.getWayStartPoint(), expected_route_start_point);

    EXPECT_EQ(shortest_distance_route.getWayEndPoint(), expected_route_end_point);
    EXPECT_EQ(shortest_time_route.getWayEndPoint(), expected_route_end_point);

    NavigationValidator navigation_validator{gis};

    EXPECT_TRUE(navigation_validator.validateRoute(start, end, shortest_distance_route));
    EXPECT_TRUE(navigation_validator.validateRoute(start, end, shortest_time_route));
}

TEST(GISEx2, NavigationTestFastestDifferentFromShortestHighway) {
    auto gis = GISProvider::getGIS();
    std::string input_file{"ex2_map2_highways.json"};
    auto loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));

    NavigationGIS navigation_gis{gis};
    auto navigation = NavigationProvider::getNavigation(navigation_gis);

    Coordinates start{ex2_map2_J1}; // Note: this point is on a way
    Coordinates end{ex2_map2_J5}; // Note: this point is on a way
    auto routes = navigation.getRoutes(start, end);
    EXPECT_TRUE(routes->isValid());

    const AbstractRoute& shortest_distance_route = routes->shortestDistance();
    const AbstractRoute& shortest_time_route = routes->shortestTime();

    EXPECT_EQ(shortest_distance_route.getWays(), ex2_map2_shortest_route);
    EXPECT_EQ(shortest_time_route.getWays(), ex2_map2_fastest_route);

    Coordinates expected_route_start_point = CoordinatesMath::closestPointOnSegment(start, ex2_map2_W1.from, ex2_map2_W1.to);
    Coordinates expected_route_end_point = CoordinatesMath::closestPointOnSegment(end, ex2_map2_W5.from, ex2_map2_W5.to);


    EXPECT_EQ(shortest_distance_route.getWayStartPoint(), expected_route_start_point);
    EXPECT_EQ(shortest_time_route.getWayStartPoint(), expected_route_start_point);

    EXPECT_EQ(shortest_distance_route.getWayEndPoint(), expected_route_end_point);
    EXPECT_EQ(shortest_time_route.getWayEndPoint(), expected_route_end_point);

    NavigationValidator navigation_validator{gis};

    EXPECT_TRUE(navigation_validator.validateRoute(start, end, shortest_distance_route));
    EXPECT_TRUE(navigation_validator.validateRoute(start, end, shortest_time_route));
}

TEST(GISEx2, NavigationTestFastestDifferentFromShortestNotOnHighway) {
    auto gis = GISProvider::getGIS();
    std::string input_file{"ex2_map2_highways.json"};
    auto loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));

    NavigationGIS navigation_gis{gis};
    auto navigation = NavigationProvider::getNavigation(navigation_gis);

    Coordinates start{Longitude{-30}, Latitude{50}}; // Note: this point is not on a way (could be on way in other cases)
    Coordinates end{Longitude{-28.8}, Latitude{47.2}}; // Note: this point is not on a way (could be on way in other cases)
    auto routes = navigation.getRoutes(start, end);
    EXPECT_TRUE(routes->isValid());

    const AbstractRoute& shortest_distance_route = routes->shortestDistance();
    const AbstractRoute& shortest_time_route = routes->shortestTime();

    EXPECT_EQ(shortest_distance_route.getWays(), ex2_map2_highways_no_highways_route);
    EXPECT_EQ(shortest_time_route.getWays(), ex2_map2_highways_no_highways_route);

    Coordinates expected_route_start_point = CoordinatesMath::closestPointOnSegment(start, ex2_map2_W2.from, ex2_map2_W2.to);
    Coordinates expected_route_end_point = CoordinatesMath::closestPointOnSegment(end, ex2_map2_W3.from, ex2_map2_W3.to);

    EXPECT_EQ(shortest_distance_route.getWayStartPoint(), expected_route_start_point);
    EXPECT_EQ(shortest_time_route.getWayStartPoint(), expected_route_start_point);

    EXPECT_EQ(shortest_distance_route.getWayEndPoint(), expected_route_end_point);
    EXPECT_EQ(shortest_time_route.getWayEndPoint(), expected_route_end_point);

    NavigationValidator navigation_validator{gis};

    EXPECT_TRUE(navigation_validator.validateRoute(start, end, shortest_distance_route));
    EXPECT_TRUE(navigation_validator.validateRoute(start, end, shortest_time_route));
}

TEST(GISEx2, NavigationTestFastestDifferentNoRoutes) {
    auto gis = GISProvider::getGIS();
    std::string input_file{"ex2_map3_uniderectional_edges.json"};
    auto loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));

    NavigationGIS navigation_gis{gis};
    auto navigation = NavigationProvider::getNavigation(navigation_gis);

    Coordinates start{ex2_map2_P1}; // Note: this point is not on a way (could be on way in other cases)
    Coordinates end{ex2_map2_P2}; // Note: this point is not on a way (could be on way in other cases)
    auto routes = navigation.getRoutes(start, end);
    EXPECT_FALSE(routes->isValid());
}

TEST(GISEx2, NavigationTestCurves2) {
    auto gis = GISProvider::getGIS();
    std::string input_file{"ex2_map4_curves2.json"};
    gis.loadMapFile(input_file);

    NavigationGIS navigation_gis{gis};
    auto navigation = NavigationProvider::getNavigation(navigation_gis);

    Coordinates start{ex2_map2_P1}; // Note: this point is not on a way (could be on way in other cases)
    Coordinates end{ex2_map2_P2}; // Note: this point is not on a way (could be on way in other cases)
    auto routes = navigation.getRoutes(start, end);
    EXPECT_TRUE(routes->isValid());

    const AbstractRoute& shortest_distance_route = routes->shortestDistance();
    const AbstractRoute& shortest_time_route = routes->shortestTime();
    EXPECT_EQ(shortest_distance_route.getWays(), ex2_map4_curves2_shortest_route);
    EXPECT_EQ(shortest_time_route.getWays(), ex2_map4_curves2_fastest_route);

    Coordinates expected_route_start_point = CoordinatesMath::closestPointOnSegment(start, ex2_map2_W1.from, ex2_map2_W1.to);
    Coordinates expected_route_end_point = CoordinatesMath::closestPointOnSegment(end, ex2_map2_W5.from, ex2_map2_W5.to);

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

TEST(GISEx2, NavigationTestCurves2TotalLength) {
    auto gis = GISProvider::getGIS();
    std::string input_file{"ex2_map4_curves2.json"};
    gis.loadMapFile(input_file);

    NavigationGIS navigation_gis{gis};
    auto navigation = NavigationProvider::getNavigation(navigation_gis);

    Coordinates start{ex2_map2_J1};
    Coordinates end{ex2_map2_J5};
    auto routes = navigation.getRoutes(start, end);
    EXPECT_TRUE(routes->isValid());

    const AbstractRoute& shortest_distance_route = routes->shortestDistance();

    EXPECT_EQ(shortest_distance_route.getWays(), ex2_map4_curves2_shortest_route);

    EXPECT_EQ(shortest_distance_route.totalLength(),
              CoordinatesMath::calculateDistance(ex2_map2_J1, ex2_map2_J2) + CoordinatesMath::calculateDistance(ex2_map2_J2, ex2_map2_J3) + CoordinatesMath::calculateDistance(ex2_map2_J3, ex2_map2_J4) + CoordinatesMath::calculateDistance(ex2_map2_J4, ex2_map2_J5));

    EXPECT_EQ(shortest_distance_route.getWayStartPoint(), start);
    EXPECT_EQ(shortest_distance_route.getWayEndPoint(), end);

    NavigationValidator navigation_validator{gis};

    EXPECT_TRUE(navigation_validator.validateRoute(start, end, shortest_distance_route));
}

TEST(GISEx2, NavigationTestCurves2EstimatedDuration) {
    constexpr double km_h_to_minutes = 0.060;

    auto gis = GISProvider::getGIS();
    std::string input_file{"ex2_map4_curves2.json"};
    gis.loadMapFile(input_file);

    NavigationGIS navigation_gis{gis};
    auto navigation = NavigationProvider::getNavigation(navigation_gis);

    Coordinates start{ex2_map2_J1};
    Coordinates end{ex2_map2_J5};
    auto routes = navigation.getRoutes(start, end);
    EXPECT_TRUE(routes->isValid());

    const AbstractRoute& fastest_route = routes->shortestTime();

    EXPECT_EQ(fastest_route.getWays(), ex2_map4_curves2_fastest_route);

    EXPECT_EQ(fastest_route.getWayStartPoint(), start);
    EXPECT_EQ(fastest_route.getWayEndPoint(), end);

    Minutes expected_duration{km_h_to_minutes*static_cast<double>(CoordinatesMath::calculateDistance(ex2_map2_J1, ex2_map2_J2)/90 + CoordinatesMath::calculateDistance(ex2_map2_J2, ex2_map2_J3)/100 + CoordinatesMath::calculateDistance(ex2_map2_J3, ex2_map4_J6)/130 + CoordinatesMath::calculateDistance(ex2_map4_J6, ex2_map2_J4)/130 + CoordinatesMath::calculateDistance(ex2_map2_J4, ex2_map2_J5)/90)};

    EXPECT_EQ(fastest_route.estimatedDuration(), expected_duration);

    NavigationValidator navigation_validator{gis};

    EXPECT_TRUE(navigation_validator.validateRoute(start, end, fastest_route));
}

