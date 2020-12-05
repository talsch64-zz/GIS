# GIS
GIS (Geographic Information System) Project for Tel Aviv University's Advanced topics in programming course.    


## Makefile
We have a dynamic makefile that searches the different folders for depenencies and automatically builds the project.
  

## Optional and required fields in Entities
 * Optional fields are wrapped in a `std::optional` container both for readabilty and usabilty, if an optional does not exist it does not get serialized/deserialized.  
 * Required fields are never wrapped in an optional, and are checked using `ObjectChecker::checkRequiredField`, which throws and appropriate exception if it does not exist (more on exceptions and loggin later).

## GIS
members of GIS are:
  * std::unordered_map<EntityId, std::unique_ptr<Entity>> entities- unordered_map which maps entities ids to unique_ptrs of the entities themselves.
  * std::shared_ptr<Grid> grid - Grid which we store all the entities on.
  * EntityJsonSerializer entityJsonSerializer - serializes entities into the correct JSON format.
  * std::shared_ptr<EntityJsonParser> entityJsonParser- parses the JSON Entities.
  * JsonFileWriter jsonFileWriter - writes the GIS content into a JSON file.
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
 * Contains grid-cells that divide the GIS map into areas by coordinates (grid-cell precision is 0.01)
 * Poles Handling: There is only one cell for each poll with Longitude of 0 - northern cell is (0, 90) and southern is (0, -90)
 
## Grid insertions:

###Adding a way
For adding a way to the grid we need to check all of it's lines, for each line:
* We split the line into two lines and call the two parts recursively   
* If the segment we split is completley inside a cell or the distance between the two edges is small enough we stop the splitting and adding the Way's id to the corresponding cells.

###Adding a point 
We truncate the point to the lower left corner (using floor for truncating)

### Adding a Circle
For adding a Circle to the grid we use DFS-like algorithms that spreads to through neighboring cells and add the entitie's id to all the cells that the distnace from the center are less or equal the radius of the circle.
Once the distnace of the current cell is larger then the radius, we no longer spreading to its neighbors.
We use unordered_set to track the cells visited so far to avoid duplicates.


## Error logging

## Tests




