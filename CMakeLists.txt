cmake_minimum_required(VERSION 3.15)
project(ex1)
include_directories(include)

set(CMAKE_CXX_STANDARD 20)

add_executable(ex1 main.cpp GIS.cpp GIS.h Entity.cpp Entity.h POI.cpp POI.h Geometry.h Circle.cpp Circle.h Junction.cpp Junction.h Way.cpp Way.h EntityJsonParser.cpp EntityJsonParser.h Geometry.cpp GISdefs.h GeometryJsonParser.cpp GeometryJsonParser.h Polygon.cpp Polygon.h)