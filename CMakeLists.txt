cmake_minimum_required(VERSION 3.15)
project(ex1)
include_directories(include)

set(CMAKE_CXX_STANDARD 20)

add_executable(ex1 main.cpp GIS.cpp GIS.h entities/Entity.cpp entities/Entity.h entities/POI.cpp entities/POI.h entities/geometry/Geometry.h entities/geometry/Circle.cpp entities/geometry/Circle.h entities/Junction.cpp entities/Junction.h entities/Way.cpp entities/Way.h entities/EntityJsonParser.cpp entities/EntityJsonParser.h entities/geometry/Geometry.cpp GISdefs.h entities/geometry/GeometryJsonParser.cpp entities/geometry/GeometryJsonParser.h entities/geometry/Polygon.cpp entities/geometry/Polygon.h)