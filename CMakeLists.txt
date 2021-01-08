cmake_minimum_required(VERSION 3.15)
project(ex1)
include_directories(include)

#TODO: uncomment
#set(CMAKE_CXX_FLAGS "-std=c++20 -Werror -Wall -Wextra -pedantic-errors -DNDEBUG")
#TODO delete
set(CMAKE_CXX_FLAGS "-std=c++20 -DNDEBUG")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_LIBRARIES -ldl)

add_executable(ex1 GIS/GIS_315524694.cpp GIS/GIS_315524694.h UserCommon/entities/Entity.cpp UserCommon/entities/Entity.h UserCommon/entities/POI.cpp UserCommon/entities/POI.h UserCommon/entities/geometry/Geometry.h UserCommon/entities/geometry/Circle.cpp UserCommon/entities/geometry/Circle.h UserCommon/entities/Junction.cpp UserCommon/entities/Junction.h UserCommon/entities/Way.cpp UserCommon/entities/Way.h UserCommon/entities/JsonHandlers/parsers/EntityJsonParser.cpp UserCommon/entities/JsonHandlers/parsers/EntityJsonParser.h UserCommon/entities/geometry/Geometry.cpp Common/GISNamedTypes.h UserCommon/entities/JsonHandlers/parsers/GeometryJsonParser.cpp UserCommon/entities/JsonHandlers/parsers/GeometryJsonParser.h UserCommon/entities/JsonHandlers/parsers/CoordinatesJsonParser.cpp UserCommon/entities/JsonHandlers/parsers/CoordinatesJsonParser.h UserCommon/entities/JsonHandlers/Serializers/EntityJsonSerializer.cpp UserCommon/entities/JsonHandlers/Serializers/EntityJsonSerializer.h UserCommon/entities/JsonHandlers/Serializers/JsonFileWriter.cpp UserCommon/entities/JsonHandlers/Serializers/JsonFileWriter.h UserCommon/entities/JsonHandlers/Serializers/JsonFileWriter.cpp UserCommon/entities/JsonHandlers/Serializers/JsonFileWriter.h UserCommon/entities/JsonHandlers/Serializers/JsonFileWriter.cpp UserCommon/entities/JsonHandlers/Serializers/JsonFileWriter.h UserCommon/entities/JsonHandlers/Serializers/GeometryJsonSerializer.cpp UserCommon/entities/JsonHandlers/Serializers/GeometryJsonSerializer.h UserCommon/entities/JsonHandlers/Serializers/CoordinatesJsonSerializer.cpp UserCommon/entities/JsonHandlers/Serializers/CoordinatesJsonSerializer.h GIS/search/Grid.h Common/NamedType.h Common/CoordinatesMath.h GIS/search/Grid.cpp GIS/search/TopologicalSearch.cpp GIS/search/TopologicalSearch.h UserCommon/IdGenerator.h UserCommon/entities/geometry/PointList.cpp UserCommon/entities/geometry/PointList.h UserCommon/entities/geometry/Point.cpp UserCommon/entities/geometry/Point.h Common/Double.h Common/NamedType.h UserCommon/Logger.cpp UserCommon/Logger.h Navigation/Route.cpp Navigation/Route.h Navigation/Routes.cpp Navigation/Routes.h Navigation/Navigation_315524694.cpp Navigation/Navigation_315524694.h Common/NavigationGIS.h Common/Restrictions.h Navigation/AStar.cpp Navigation/AStar.h UserCommon/Utils.h tests/NavigationValidator.h tests/NavigationValidator.cpp
        UserCommon/Utils.cpp Common/AbstractGIS.h Common/AbstractNavigation.h Common/AbstractRoute.h Common/AbstractRoutes.h Common/AbstractWay.h Common/NavigationRegistration.h Common/GISRegistration.h Simulator/GISRegistration.cpp Simulator/NavigationRegistration.cpp Simulator/main.cpp)

add_executable(simulator Simulator/main.cpp Simulator/GISRegistration.cpp Simulator/NavigationRegistration.cpp Common/GISRegistration.h Simulator/Simulation.cpp Simulator/Simulation.h Simulator/GISContainer.cpp Simulator/GISContainer.h Simulator/NavigationContainer.cpp Simulator/NavigationContainer.h Simulator/Registrar.cpp Simulator/Registrar.h)
set_target_properties(simulator PROPERTIES LINK_FLAGS "-rdynamic -pthread -ldl")

add_subdirectory(tests)

configure_file(GIS/GIS_315524694.so GIS/GIS_315524694.so COPYONLY)
configure_file(Navigation/Navigation_315524694.so Navigation/Navigation_315524694.so COPYONLY)