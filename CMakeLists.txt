cmake_minimum_required(VERSION 3.15)
project(ex1)
include_directories(include)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -pedantic-errors -Werror" )
set(CMAKE_CXX_STANDARD 20)

add_executable(ex1 main.cpp GIS.cpp GIS.h entities/Entity.cpp entities/Entity.h entities/POI.cpp entities/POI.h entities/geometry/Geometry.h entities/geometry/Circle.cpp entities/geometry/Circle.h entities/Junction.cpp entities/Junction.h entities/Way.cpp entities/Way.h entities/EntityJsonParser.cpp entities/EntityJsonParser.h entities/geometry/Geometry.cpp GISdefs.h entities/geometry/GeometryJsonParser.cpp entities/geometry/GeometryJsonParser.h entities/geometry/Polygon.cpp entities/geometry/Polygon.h entities/CoordinatesJsonParser.cpp entities/CoordinatesJsonParser.h entities/JsonHandlers/Serializers/EntityJsonSerializer.cpp entities/JsonHandlers/Serializers/EntityJsonSerializer.h entities/JsonHandlers/Serializers/JsonFileWriter.cpp entities/JsonHandlers/Serializers/JsonFileWriter.h entities/JsonHandlers/Serializers/JsonFileWriter.cpp entities/JsonHandlers/Serializers/JsonFileWriter.h entities/JsonHandlers/Serializers/JsonFileWriter.cpp entities/JsonHandlers/Serializers/JsonFileWriter.h entities/JsonHandlers/Serializers/GeometryJsonSerializer.cpp entities/JsonHandlers/Serializers/GeometryJsonSerializer.h entities/JsonHandlers/Serializers/CoordinatesJsonSerializer.cpp entities/JsonHandlers/Serializers/CoordinatesJsonSerializer.h search/Grid.h NamedType.h entities/geometry/CoordinatesMath.h search/Grid.cpp search/TopologicalSearch.cpp search/TopologicalSearch.h IdGenerator.h entities/geometry/PointList.cpp entities/geometry/PointList.h entities/geometry/Point.cpp entities/geometry/Point.h Double.h NamedType.h Logger.cpp Logger.h)

add_subdirectory(tests)