

#ifndef GIS_CPP_ASTAR_H
#define GIS_CPP_ASTAR_H

#include <queue>
#include <unordered_map>
#include <functional>
#include <memory>
#include "../Common/GISNamedTypes.h"
#include "../entities/Way.h"
#include "../Common/NavigationGIS.h"
#include "../Common/CoordinatesMath.h"
#include "Route.h"

#define MAX_SPEED 130.0


/**
 * @brief AStar class implements the A* algorithm.
 * Allows to find the shortest Route from origin point to destination point By distance and by time.
 * We implemented the algorithm such that the initial Node already has a "kilometrage" of the startWay.
 * If the first way is bidirectional then we initialize two node.
 * Once one of the Junctions of the final way is reached (if the way is bidirectional), we retract the route that led to the way.
 * If the final way is unidirectional, the algorithm retracts the Route after finding the "from" junction of the final way.
 * Supports restrictions!!!!!

 */
class AStar {
    const NavigationGIS &navigationGIS;
    const Coordinates origin;
    const Coordinates destination;
    const AbstractWay &startWay;
    const AbstractWay &finalWay;
    Restrictions restrictions;

public:
    /**
     *
     * @param navigationGis
     * @param origin
     * @param destination
     * @param startWay the way which the origin point is located on.
     * @param finalWay the way which the destination point is located on.
     */
    AStar(const NavigationGIS &navigationGis, const Coordinates &origin, const Coordinates &destination,
          const AbstractWay &startWay, const AbstractWay &finalWay);


    /**
     *
     * @param navigationGis
     * @param origin
     * @param destination
     * @param startWay the way which the origin point is located on.
     * @param finalWay the way which the destination point is located on.
     * @param restrictions
     */
    AStar(const NavigationGIS &navigationGis, const Coordinates &origin, const Coordinates &destination,
          const AbstractWay &startWay, const AbstractWay &finalWay, const Restrictions &restrictions);

    Route shortestByDistance();

    Route shortestByTime();

private:
    using Edge = std::optional<std::pair<EntityId, Direction>>;

    /**
     * class Node represents a node in the A* algorithm
     */
    class Node {
        Coordinates coordinates;
        EntityId junctionId;
        Meters distanceSoFar;
        Minutes timeSoFar;
        double costSoFar;
        // priority(n) = cost(n) + heuristicFunc(n)
        double priority;
        unsigned int waysCount;
        Edge prevEdgeWay;
        std::shared_ptr<Node> prevNode;

    public:
        Node(const Coordinates &coordinates, const EntityId &junctionId, const Meters &distanceSoFar,
             const Minutes &timeSoFar, double costSoFar,
             double priority, unsigned int waysCount, const Edge &prevEdgeWay, std::shared_ptr<Node> prevNode);


        const Edge &getPrevEdgeWay() const;

        double getCostSoFar() const;

        const Coordinates &getCoordinates() const;

        double getPriority() const;

        const EntityId &getJunctionId() const;

        const Meters &getDistanceSoFar() const;

        const Minutes &getTimeSoFar() const;

        unsigned int getWaysCount() const;

        const std::shared_ptr<Node> &getPrevNode() const;


    };

    /**
     * @brief heuristic by distance
     * @param start start coordinates
     * @return the aerial distance between the coordinates and the destination of the search
     */
    static double distanceHeuristic(const Coordinates &coordinates, const Coordinates &destination);

    /**
     * @brief heuristic by Time
     * @param coordinates start coordinates
     * @return the time it takes to get start coordinates to the destination coordinates in aerial distance with max speed (130 kmh)
     */
    static double timeHeuristic(const Coordinates &coordinates, const Coordinates &destination);

    /**
     * @brief calculate the cost of the way by distance in meters
     * @param way
     * @return the length of the way
     */
    static double costByDistance(const AbstractWay &way);

    /**
     * @brief calculates the cost of the way by time in minutes
     * @param way
     * @return the time to cross the way in minutes
     */
    static double costByTime(const AbstractWay &way);

    /**
     * @brief comparator function for storing Nodes in the priority_queue of A* algorithm.
     * order of comparison: lowest priority -> shortest distance -> shortest time
     * @param node1
     * @param node2
     * @return true if node1 "larger" then node2, else false
     */
    static bool compareByDistance(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2);

    /**
     * @brief comparator function for storing Nodes in the priority_queue of A* algorithm.
     * order of comparison: lowest priority -> shortest time -> shortest distance
     * @param node1
     * @param node2
     * @return true if node1 "larger" then node2, else false
     */
    static bool compareByTime(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2);

    Route searchShortestRoute(double (*heuristicFunc)(const Coordinates &start, const Coordinates &target),
                              double (*costFunc)(const AbstractWay &way),
                              bool (*comparator)(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2));

    /**
     *
     * @param node final Node of the search
     * @return all the ways of the Route by order
     */
    std::vector<std::pair<EntityId, Direction>> restoreShortestRoute(std::shared_ptr<Node> node);

    /**
     * @brief creates the neighbor node to the current node
     * @param currNode
     * @param wayId the way's id that connects the current node and the neighbor
     * @param heuristicFunc
     * @param costFunc
     * @return
     */
    std::shared_ptr<Node> createNeighbor(std::shared_ptr<Node> currNode, EntityId wayId,
                                         double (*heuristicFunc)(const Coordinates &start, const Coordinates &end),
                                         double (*costFunc)(const AbstractWay &));

    /**
     * @brief initializes the first Node according to the given direction.
     * @param heuristicFunc
     * @param costFunc
     * @param direction if direction is A_to_B initialize node that represents "to" junction, else "from" junction.
     * @return initialNode
     */
    std::shared_ptr<Node> createInitialNode(double (*heuristicFunc)(const Coordinates &start, const Coordinates &end),
                                            double (*costFunc)(const AbstractWay &), Direction direction);

    /**
     * @brief initializes the final Node! represent the destination point and not a junction!
     * @param currNode node that represents a junction of the final way.
     * @param heuristicFunc
     * @param costFunc
     * @return final node :)
     */
    std::shared_ptr<Node> createFinalNode(std::shared_ptr<Node> currNode, double (*costFunc)(const AbstractWay &));


};

#endif //GIS_CPP_ASTAR_H
