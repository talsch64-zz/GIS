cmake_minimum_required(VERSION 3.15)
project(ex1)
include_directories(include)

#TODO: uncomment
set(CMAKE_CXX_FLAGS "-std=c++20 -Werror -Wall -Wextra -pedantic-errors -DNDEBUG")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_LIBRARIES -ldl)

add_executable(ex1 GIS/GIS_315524694.cpp GIS/GIS_315524694.h GIS/entities/Entity.cpp GIS/entities/Entity.h GIS/entities/POI.cpp GIS/entities/POI.h GIS/entities/geometry/Geometry.h GIS/entities/geometry/Circle.cpp GIS/entities/geometry/Circle.h GIS/entities/Junction.cpp GIS/entities/Junction.h GIS/entities/Way.cpp GIS/entities/Way.h GIS/entities/JsonHandlers/parsers/EntityJsonParser.cpp GIS/entities/JsonHandlers/parsers/EntityJsonParser.h GIS/entities/geometry/Geometry.cpp Common/GISNamedTypes.h GIS/entities/JsonHandlers/parsers/GeometryJsonParser.cpp GIS/entities/JsonHandlers/parsers/GeometryJsonParser.h GIS/entities/JsonHandlers/parsers/CoordinatesJsonParser.cpp GIS/entities/JsonHandlers/parsers/CoordinatesJsonParser.h GIS/entities/JsonHandlers/Serializers/EntityJsonSerializer.cpp GIS/entities/JsonHandlers/Serializers/EntityJsonSerializer.h GIS/entities/JsonHandlers/Serializers/JsonFileWriter.cpp GIS/entities/JsonHandlers/Serializers/JsonFileWriter.h GIS/entities/JsonHandlers/Serializers/JsonFileWriter.cpp GIS/entities/JsonHandlers/Serializers/JsonFileWriter.h GIS/entities/JsonHandlers/Serializers/JsonFileWriter.cpp GIS/entities/JsonHandlers/Serializers/JsonFileWriter.h GIS/entities/JsonHandlers/Serializers/GeometryJsonSerializer.cpp GIS/entities/JsonHandlers/Serializers/GeometryJsonSerializer.h GIS/entities/JsonHandlers/Serializers/CoordinatesJsonSerializer.cpp GIS/entities/JsonHandlers/Serializers/CoordinatesJsonSerializer.h GIS/search/Grid.h Common/NamedType.h Common/CoordinatesMath.h GIS/search/Grid.cpp GIS/search/TopologicalSearch.cpp GIS/search/TopologicalSearch.h UserCommon/IdGenerator.h GIS/entities/geometry/PointList.cpp GIS/entities/geometry/PointList.h GIS/entities/geometry/Point.cpp GIS/entities/geometry/Point.h Common/Double.h Common/NamedType.h UserCommon/Logger.cpp UserCommon/Logger.h Navigation/Route.cpp Navigation/Route.h Navigation/Routes.cpp Navigation/Routes.h Navigation/Navigation_315524694.cpp Navigation/Navigation_315524694.h Common/NavigationGIS.h Common/Restrictions.h Navigation/AStar.cpp Navigation/AStar.h UserCommon/Utils.h tests/NavigationValidator.h tests/NavigationValidator.cpp
        UserCommon/Utils.cpp Common/AbstractGIS.h Common/AbstractNavigation.h Common/AbstractRoute.h Common/AbstractRoutes.h Common/AbstractWay.h Common/NavigationRegistration.h Common/GISRegistration.h Simulator/GISRegistration.cpp Simulator/NavigationRegistration.cpp Simulator/main.cpp)

add_executable(simulator Simulator/main.cpp Simulator/GISRegistration.cpp Simulator/NavigationRegistration.cpp Common/GISRegistration.h)
set_target_properties(simulator PROPERTIES LINK_FLAGS "-rdynamic -pthread -ldl")

add_subdirectory(tests)

configure_file(GIS/GIS_315524694.so GIS/GIS_315524694.so COPYONLY)