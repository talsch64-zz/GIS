# GIS - Advanced Topics in Programing
GIS (Geographic Information System) Project for Tel Aviv University's Advanced topics in programming course.    


# Project 1 - GIS
Stores data about geographic entities (POI's, ways and junctions).
Enables loading maps from json formats, finding the closest entities from a given coordinates, finding the closest way and the closest point on that way from a given coordinates and much more cool features :)

## General Notes:
### Optional and required fields in Entities
 * Optional fields are wrapped in a `std::optional` container both for readabilty and usabilty, if an optional does not exist it does not get serialized/deserialized.  
 * Required fields are never wrapped in an optional, and are checked using `ObjectChecker::checkRequiredField`, which throws and appropriate exception if it does not exist (more on exceptions and loggin later).

## Classes And Utilities
### class GIS
members of GIS are:
  * std::unordered_map<EntityId, std::unique_ptr<Entity>> entities- unordered_map which maps entities ids to unique_ptrs of the entities themselves.
  * std::shared_ptr<Grid> grid - Grid which we store all the entities on.
  * EntityJsonSerializer entityJsonSerializer - serializes entities into the correct JSON format.
  * std::shared_ptr<EntityJsonParser> entityJsonParser- parses the JSON Entities.
  * JsonFileWriter jsonFileWriter - writes the GIS content into a JSON file.
  * std::unique_ptr<TopologicalSearch> topologicalSearch - executes searches on the grid.
  * std::unique_ptr<Logger> logger - Error handling and and logging errors.


### class Entity
  * Abstract Class with three derived classes: POI, Way, Junction
  * Each derived class has Geometry member (see Geometry below)
	

### class Geometery
  * Abstarct class.
  * Our code supports 3 geometry types:
	- Circle: radius in Meters and center Coordinates (used as POI's Geometry)
	- PointList: vector of Coordinates that represents different segments of a Way (used as Way's Geometry)
	- Point: Coordinates (used as the Junction's Geometry)


### Parsers:
  * EntityJsonParser - parses JSON Entities
  * GeometryParser - parses the geometry of the different entities
  * CoordinatesParse - parses Coordinates from JSON files

### Serializers:
  * EntityJsonSerializer - serializes entities to JSON format
  * GeometryJsonSerializer - serializes entitie's geometry to JSON format
  * CoordinatesJsonSerializer -  serializes Coordinates to JSON format


### class Grid	
 * Contains grid-cells that divide the GIS map into areas by coordinates (grid-cell precision is 0.01)
 * Poles Handling: There is only one cell for each poll with Longitude of 0 - northern cell is (0, 90) and southern is (0, -90)
 
#### Grid insertions:

##### Adding a way
For adding a way to the grid we need to check all of it's lines, for each line:
* We split the line into two lines and call the two parts recursively   
* If the segment we split is completley inside a cell or the distance between the two edges is small enough we stop the splitting and adding the Way's id to the corresponding cells.

##### Adding a point 
We truncate the point to the lower left corner (using floor for truncating)

##### Adding a Circle
For adding a Circle to the grid we use DFS-like algorithms that spreads to through neighboring cells and add the entitie's id to all the cells that the distnace from the center are less or equal the radius of the circle.
Once the distnace of the current cell is larger then the radius, we no longer spreading to its neighbors.
We use unordered_set to track the cells visited so far to avoid duplicates.

### class TopologicalSearch
Handles geographical searches including searching for grid cells in a given circle.

### class Logger
Handles errors which arise during the JSON parsing and logs them in a logger file.

# Project 2 - Navigation
Manages Navigation between two coordinates - enables to find the fastest and shortest routes.

## Classes And Utilities
### class Navigation
Handles navigation queries - manages the search for the shortest route and the fastest route.
Supports also queries with custom restrictions such as avoid highway or toll roads.

### class Route
Route class describes a route between two points and the ways in which the route goes through.
* Information available about the Route:
* Distance (in Meters),
* Duration (in Minutes),
* Start and end points,
* All the ways in which the route goes through (and the directions),
* Is the route valid.

### class Routes
Routes class hold the two shortest routes - by distance and time.
* Operation supported:
* get shortest route by distance and by time,
* are the shortest routes valid,
* get error message in case the routes are not valid.

### class NavigationValidator
Validates if a route is valid.

### class AStar
AStar class implements the A* algorithm.
 * Allows to find the shortest Route from origin point to destination point By distance and by time.
 * We implemented the algorithm such that the initial Node already has a "kilometrage" of the startWay.
 * If the first way is bidirectional then we initialize two node.
 * Once one of the Junctions of the final way is reached (if the way is bidirectional), we retract the route that led to the way.
 * If the final way is unidirectional, the algorithm retracts the Route after finding the "from" junction of the final way.
 * Supports restrictions!!!!!
 
# Project 3 - Simulator
The third project simulates a competition between the varioues GIS'es and Navigation algorithms of the different teams.
Each team provides two shared libraries (.so files) - GIS and Navigation.
The Simulator loads the different .so files and simulates navigation requests (given as an input) on all combination of Navigation x GIS and generates a score table and log of strange result. 
The highest score is given to the best Navigation algorithm (finds the shortest and fastest routes) and the minimal usage of requests from the GIS.
The Simulator supports multi-threading and enables to run various Navigation tasks concurrently.
 
 
## General Notes:
The directory tree of the Simulator project is divided into four sub-directories:
 * Common - includes the public API of the differesnt abstarct classes all the teams share between them (e.g. CoordinatesMath, AbstractGIS, AbsractNavigation, etc.)
 * GIS - includes all files relevent to the GIS project
 * Navigation - includes all files relevent to the Navigation project
 * UserCommon - includes files relevant to more then one project (e.g Utils.h and Utils.cpp)

The binary file excpects the followng command line arguments:
* Number of threads for the program to run with
* Directory path to both Navigation and GIS .so file
* Directory path of the output files
* Map file path.
* Navigation requests file path.

In order to minimize memory usage, the number of loaded gis concurrently is kept in minimum.

 
## Classes And Utilities:
We chose to highlight the following classes:
 
### class Simulation
* class Simulation simulates the various navigation and gis .so files on a given map file and navigation requests and analyzes the results.
* Implemented as a Singleton.
 
### class Registrar
* class Registrar handles .so registration and command line arguments parsing.


### class NavigationTask
* Represents a Navigation x GIS x request combination.
 
### class NavigationTaskManager
* Responsible for providing NavigationTasks and emulates an iterator.
* TaskManager maximizes gis utilization: while there are Navigations and requests remaining, it creates tasks with
* the same gis. This feature helps reduce memory usage because because it minimizes the number of gises loaded
* concurrently.



 
 
 
