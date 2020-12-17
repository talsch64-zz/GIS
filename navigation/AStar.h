

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
    double distanceHeuristic(const Coordinates &start, const Coordinates &target, const Way &way) {
        return (double) CoordinatesMath::calculateDistance(start, target);
    }

    /**
     * @brief heuristic by Time
     * @param start start coordinates
     * @param target target coordinates
     * @return the time it takes to get from start coordinates to target coordinates in aerial distance with max speed (130 kmh)
     */
    double timeHeuristic(const Coordinates &start, const Coordinates &target) {
        // meter per hour
        return (double) CoordinatesMath::calculateDistance(start, target) / kmh_to_mh(MAX_SPEED);
    }

    /**
     * @brief calculate the cost of the way by distance in meters
     * @param way
     * @return the length of the way
     */
    double costByDistance(const Way &way) {
        return (double) CoordinatesMath::calculateDistance(way.getFromJunctionCoordinates(),
                                                           way.getToJunctionCoordinates());
    }

    /**
     * @brief calculates the cost of the way by time in minutes
     * @param way
     * @return the time to cross the way in minutes
     */
    double costByTime(const Way &way) {
        return (double) way.getLength() / kmh_to_mh(way.getSpeedLimit());
    }

    /**
     * @brief comparator function for storing Nodes in the priority_queue of A* algorithm.
     * order of comparison: lowest priority -> shortest distance -> shortest time
     * @param node1
     * @param node2
     * @return true if node1 "larger" then node2, else false
     */
    bool compareByDistance(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
        if (node1->getPriority() == node2->getPriority()) {
            if (node1->getDistanceSoFar() == node2->getDistanceSoFar()) {
                return node1->getTimeSoFar() > node2->getTimeSoFar() ? true : false;
            }
            return node1->getDistanceSoFar() > node2->getDistanceSoFar();
        }
        return node1->getPriority() > node2->getPriority();
    }

    /**
     * @brief comparator function for storing Nodes in the priority_queue of A* algorithm.
     * order of comparison: lowest priority -> shortest time -> shortest distance
     * @param node1
     * @param node2
     * @return true if node1 "larger" then node2, else false
     */
    bool compareByTime(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
        if (node1->getPriority() == node2->getPriority()) {
            if (node1->getTimeSoFar() == node2->getTimeSoFar()) {
                return node1->getDistanceSoFar() > node2->getDistanceSoFar() ? true : false;
            }
            return node1->getTimeSoFar() > node2->getTimeSoFar();
        }
        return node1->getPriority() > node2->getPriority();
    }

    double kmh_to_mh(int speed) {
        return ((double) speed * 1000) / 60;
    }

    const Route &search(Way *startWay, Way *finalWay, Coordinates start, Coordinates destination, NavigationGIS navigationGIS,
                double (*heuristicFunc)(const Coordinates &start, const Coordinates &target),
                double (*costFunc)(const Way &way), double (*comparator)(std::shared_ptr<Node>node1, std::shared_ptr<Node>node2));

    std::vector<std::pair<EntityId, Direction>> restoreShortestRoute(std::shared_ptr<Node> node);
};

//TODO delete
//bool operator<(const Node &other) {
//    if (cost.has_value()) {
//        return other.cost.has_value() ? cost.value() > other.cost.value() : false;
//    } else {
//        return true;
//    }
//}
#endif //GIS_CPP_ASTAR_H
