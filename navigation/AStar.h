

#ifndef GIS_CPP_ASTAR_H
#define GIS_CPP_ASTAR_H

#include <queue>
#include <unordered_map>
#include <functional>
#include <memory>
#include "../GISNamedTypes.h"
#include "../entities/Way.h"
#include "../NavigationGIS.h"
#include "../CoordinatesMath.h"
#include "Route.h"

#define MAX_SPEED 130.0

class AStar {
public:
    AStar(const NavigationGIS &navigationGIS);

//    TODO add navigationGIS as a member
    const Route &shortestByDistance(Way *startWay, Way *finalWay, Coordinates start, Coordinates destination);
    const Route &shortestByTime(Way *startWay, Way *finalWay, Coordinates start, Coordinates destination);

private:
    const NavigationGIS &navigationGIS;
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
        // priority(n) = cost(n) + h(n) where h is the heuristic function
        double priority;
        Edge prevEdgeWay;


    public:
        Node(const Coordinates &coordinates, const EntityId &junctionId, const Meters &distanceSoFar,
             const Minutes &timeSoFar, double costSoFar,
             double priority, const Edge &prevEdgeWay);


        const Edge &getPrevEdgeWay() const;

        double getCostSoFar() const;

        const Coordinates &getCoordinates() const;

        double getPriority() const;

        const EntityId &getJunctionId() const;

        const Meters &getDistanceSoFar() const;

        const Minutes &getTimeSoFar() const;


    };

    /**
     * @brief heuristic by distance
     * @param start start coordinates
     * @param target target coordinates
     * @return the aerial distance between the two coordinates
     */
    static double distanceHeuristic(const Coordinates &start, const Coordinates &target);

    /**
     * @brief heuristic by Time
     * @param start start coordinates
     * @param target target coordinates
     * @return the time it takes to get from start coordinates to target coordinates in aerial distance with max speed (130 kmh)
     */
    static double timeHeuristic(const Coordinates &start, const Coordinates &target);

    /**
     * @brief calculate the cost of the way by distance in meters
     * @param way
     * @return the length of the way
     */
    static double costByDistance(const Way &way);

    /**
     * @brief calculates the cost of the way by time in minutes
     * @param way
     * @return the time to cross the way in minutes
     */
    static double costByTime(const Way &way);

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

    const Route &search(Way *startWay, Way *finalWay, Coordinates start, Coordinates destination,
                double (*heuristicFunc)(const Coordinates &start, const Coordinates &target),
                double (*costFunc)(const Way &way), bool (*comparator)(std::shared_ptr<Node>node1, std::shared_ptr<Node>node2));

    std::vector<std::pair<EntityId, Direction>> restoreShortestRoute(std::shared_ptr<Node> node);

    inline static double kmh_to_mh(int speed) {
        return ((double) speed * 1000) / 60;
    }
};

#endif //GIS_CPP_ASTAR_H
