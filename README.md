# GIS_315524694
GIS_315524694 (Geographic Information System) Project for Tel Aviv University's Advanced topics in programming course.    


## Makefile
We have a dynamic makefile that searches the different folders for depenencies and automatically builds the project.
  

## Optional and required fields in Entities
 * Optional fields are wrapped in a `std::optional` container both for readabilty and usabilty, if an optional does not exist it does not get serialized/deserialized.  
 * Required fields are never wrapped in an optional, and are checked using `ObjectChecker::checkRequiredField`, which throws and appropriate exception if it does not exist (more on exceptions and loggin later).

## GIS_315524694
members of GIS_315524694 are:
  * std::unordered_map<EntityId, std::unique_ptr<Entity>> entities- unordered_map which maps entities ids to unique_ptrs of the entities themselves.
  * std::shared_ptr<Grid> grid - Grid which we store all the entities on.
  * EntityJsonSerializer entityJsonSerializer - serializes entities into the correct JSON format.
  * std::shared_ptr<EntityJsonParser> entityJsonParser- parses the JSON Entities.
  * JsonFileWriter jsonFileWriter - writes the GIS_315524694 content into a JSON file.
  * std::unique_ptr<TopologicalSearch> topologicalSearch - executes searches on the grid.
  * std::unique_ptr<Logger> logger - Error handling and and logging errors.


## Entity
  * Abstract Class with three derived classes: POI, Way, Junction
  * Each derived class has Geometry member (see Geometry below)
	

## Geometery
  * Abstarct class.
  * Our code supports 3 geometry types:
	- Circle: radius in Meters and center Coordinates (used as POI's Geometry)
	- PointList: vector of Coordinates that represents different segments of a Way (used as Way's Geometry)
	- Point: Coordinates (used as the Junction's Geometry)


## Parsers:
  * EntityJsonParser - parses JSON Entities
  * GeometryParser - parses the geometry of the different entities
  * CoordinatesParse - parses Coordinates from JSON files

## Serializers:
  * EntityJsonSerializer - serializes entities to JSON format
  * GeometryJsonSerializer - serializes entitie's geometry to JSON format
  * CoordinatesJsonSerializer -  serializes Coordinates to JSON format


## Grid	
 * Contains grid-cells that divide the GIS_315524694 map into areas by coordinates (grid-cell precision is 0.01)
 * Poles Handling: There is only one cell for each poll with Longitude of 0 - northern cell is (0, 90) and southern is (0, -90)
 
## Grid insertions:

### Adding a way
For adding a way to the grid we need to check all of it's lines, for each line:
* We split the line into two lines and call the two parts recursively   
* If the segment we split is completley inside a cell or the distance between the two edges is small enough we stop the splitting and adding the Way's id to the corresponding cells.

### Adding a point 
We truncate the point to the lower left corner (using floor for truncating)

### Adding a Circle
For adding a Circle to the grid we use DFS-like algorithms that spreads to through neighboring cells and add the entitie's id to all the cells that the distnace from the center are less or equal the radius of the circle.
Once the distnace of the current cell is larger then the radius, we no longer spreading to its neighbors.
We use unordered_set to track the cells visited so far to avoid duplicates.

## TopologicalSearch
Handles geographical searches including searching for grid cells in a given circle.

## Logger
Handles errors which arise during the JSON parsing and logs them in a logger file.

## Navigation_315524694
Handles navigation queries - manages the search for the shortest route and the fastest route.
Supports also queries with custom restrictions such as avoid highway or toll roads.

## Route
Route class describes a route between two points and the ways in which the route goes through.
* Information available about the Route:
* Distance (in Meters),
* Duration (in Minutes),
* Start and end points,
* All the ways in which the route goes through (and the directions),
* Is the route valid.

## Routes
Routes class hold the two shortest routes - by distance and time.
* Operation supported:
* get shortest route by distance and by time,
* are the shortest routes valid,
* get error message in case the routes are not valid.

## NavigationValidator
Validates if a route is valid 

## AStar
AStar class implements the A* algorithm.
 * Allows to find the shortest Route from origin point to destination point By distance and by time.
 * We implemented the algorithm such that the initial Node already has a "kilometrage" of the startWay.
 * If the first way is bidirectional then we initialize two node.
 * Once one of the Junctions of the final way is reached (if the way is bidirectional), we retract the route that led to the way.
 * If the final way is unidirectional, the algorithm retracts the Route after finding the "from" junction of the final way.
 * Supports restrictions!!!!!
