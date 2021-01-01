# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(Google_Tests_run SearchTest.cpp ../GIS/GIS_315524694.cpp ../GIS/GIS_315524694.h ../GIS/entities/Entity.cpp ../GIS/entities/Entity.h ../GIS/entities/POI.cpp ../GIS/entities/POI.h ../GIS/entities/geometry/Geometry.h ../GIS/entities/geometry/Circle.cpp ../GIS/entities/geometry/Circle.h ../GIS/entities/Junction.cpp ../GIS/entities/Junction.h ../GIS/entities/Way.cpp ../GIS/entities/Way.h ../GIS/entities/JsonHandlers/parsers/EntityJsonParser.cpp ../GIS/entities/JsonHandlers/parsers/EntityJsonParser.h ../GIS/entities/geometry/Geometry.cpp ../Common/GISNamedTypes.h ../GIS/entities/JsonHandlers/parsers/GeometryJsonParser.cpp ../GIS/entities/JsonHandlers/parsers/GeometryJsonParser.h ../GIS/entities/JsonHandlers/parsers/CoordinatesJsonParser.cpp ../GIS/entities/JsonHandlers/parsers/CoordinatesJsonParser.h ../GIS/entities/JsonHandlers/Serializers/EntityJsonSerializer.cpp ../GIS/entities/JsonHandlers/Serializers/EntityJsonSerializer.h ../GIS/entities/JsonHandlers/Serializers/JsonFileWriter.cpp ../GIS/entities/JsonHandlers/Serializers/JsonFileWriter.h ../GIS/entities/JsonHandlers/Serializers/JsonFileWriter.cpp ../GIS/entities/JsonHandlers/Serializers/JsonFileWriter.h ../GIS/entities/JsonHandlers/Serializers/JsonFileWriter.cpp ../GIS/entities/JsonHandlers/Serializers/JsonFileWriter.h ../GIS/entities/JsonHandlers/Serializers/GeometryJsonSerializer.cpp ../GIS/entities/JsonHandlers/Serializers/GeometryJsonSerializer.h ../GIS/entities/JsonHandlers/Serializers/CoordinatesJsonSerializer.cpp ../GIS/entities/JsonHandlers/Serializers/CoordinatesJsonSerializer.h ../GIS/search/Grid.h ../Common/NamedType.h ../Common/CoordinatesMath.h ../GIS/search/Grid.cpp ../GIS/search/TopologicalSearch.cpp ../GIS/search/TopologicalSearch.h ../IdGenerator.h ../GIS/entities/geometry/PointList.cpp ../GIS/entities/geometry/PointList.h ../GIS/entities/geometry/Point.cpp ../GIS/entities/geometry/Point.h SearchTest.cpp ../Logger.cpp ../Logger.h GeometryTest.cpp error_handling.cpp error_handling_more.cpp GISMock.cpp GISMock.h _tests.cpp internalTests.cpp ../Common/NavigationGIS.h ../Common/Restrictions.h RestrictionsTest.cpp WayTests.cpp ../Navigation/AStar.cpp ../Navigation/AStar.h ../Navigation/Route.cpp ../Navigation/Route.h AStarTests.cpp ../Navigation/Routes.h ../Navigation/Routes.cpp ../Navigation/Navigation.h ../Navigation/Navigation.cpp ../Utils.h NavigationValidator.h NavigationValidator.cpp AStarTests2.cpp RandTestUtils.cpp RandTestUtils.h Bound.cpp Bound.h RouteMock.cpp RouteMock.h WayMock.cpp WayMock.h LoadSaveMapTests.cpp ../Utils.cpp testsEx2.cpp ../bonus.avoid_tolls/avoid_tolls.cpp
        ../Common/AbstractGIS.h ../Common/AbstractNavigation.h ../Common/AbstractRoute.h ../Common/AbstractRoutes.h ../Common/AbstractWay.h ../Common/NavigationRegistration.h ../Common/GISRegistration.h)
target_link_libraries(Google_Tests_run gtest gtest_main)

file(GLOB MAP_FILES "*.json")

foreach (CurrentFile IN LISTS MAP_FILES)
    add_custom_command(
            TARGET Google_Tests_run PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CurrentFile} .
            COMMENT "Copying map file: ${CurrentFile}")
endforeach ()