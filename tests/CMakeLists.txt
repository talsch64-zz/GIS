# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(Google_Tests_run SearchTest.cpp ../GIS.cpp ../GIS.h ../entities/Entity.cpp ../entities/Entity.h ../entities/POI.cpp ../entities/POI.h ../entities/geometry/Geometry.h ../entities/geometry/Circle.cpp ../entities/geometry/Circle.h ../entities/Junction.cpp ../entities/Junction.h ../entities/Way.cpp ../entities/Way.h ../entities/JsonHandlers/parsers/EntityJsonParser.cpp ../entities/JsonHandlers/parsers/EntityJsonParser.h ../entities/geometry/Geometry.cpp ../GISNamedTypes.h ../entities/JsonHandlers/parsers/GeometryJsonParser.cpp ../entities/JsonHandlers/parsers/GeometryJsonParser.h ../entities/JsonHandlers/parsers/CoordinatesJsonParser.cpp ../entities/JsonHandlers/parsers/CoordinatesJsonParser.h ../entities/JsonHandlers/Serializers/EntityJsonSerializer.cpp ../entities/JsonHandlers/Serializers/EntityJsonSerializer.h ../entities/JsonHandlers/Serializers/JsonFileWriter.cpp ../entities/JsonHandlers/Serializers/JsonFileWriter.h ../entities/JsonHandlers/Serializers/JsonFileWriter.cpp ../entities/JsonHandlers/Serializers/JsonFileWriter.h ../entities/JsonHandlers/Serializers/JsonFileWriter.cpp ../entities/JsonHandlers/Serializers/JsonFileWriter.h ../entities/JsonHandlers/Serializers/GeometryJsonSerializer.cpp ../entities/JsonHandlers/Serializers/GeometryJsonSerializer.h ../entities/JsonHandlers/Serializers/CoordinatesJsonSerializer.cpp ../entities/JsonHandlers/Serializers/CoordinatesJsonSerializer.h ../search/Grid.h ../NamedType.h ../CoordinatesMath.h ../search/Grid.cpp ../search/TopologicalSearch.cpp ../search/TopologicalSearch.h ../IdGenerator.h ../entities/geometry/PointList.cpp ../entities/geometry/PointList.h ../entities/geometry/Point.cpp ../entities/geometry/Point.h SearchTest.cpp ../Logger.cpp ../Logger.h GeometryTest.cpp error_handling.cpp error_handling_more.cpp GISMock.cpp GISMock.h tests.cpp _tests.cpp internalTests.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)

file(GLOB MAP_FILES "*.json")

foreach (CurrentFile IN LISTS MAP_FILES)
    add_custom_command(
            TARGET Google_Tests_run PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CurrentFile} .
            COMMENT "Copying map file: ${CurrentFile}")
endforeach ()