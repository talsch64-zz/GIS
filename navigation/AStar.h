

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

#define MAX_SPEED 130.0

class AStar {

public:
    using Edge = std::optional<std::pair<EntityId, Direction>>;

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

        void setCost(double _cost);

        void setPriority(double _priority);

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

    double costByDistance(Way &way) {
        return (double) CoordinatesMath::calculateDistance(way.getFromJunctionCoordinates(),
                                                           way.getToJunctionCoordinates());
    }

    double costByTime(Way &way) {
        return (double) way.getLength() / kmh_to_mh(way.getSpeedLimit());
    }

    bool compareByDistance(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
        if (node1->getPriority() == node2->getPriority()) {
            if (node1->getDistanceSoFar() == node2->getDistanceSoFar()) {
                return node1->getTimeSoFar() > node2->getTimeSoFar() ? true : false;
            }
            return node1->getDistanceSoFar() > node2->getDistanceSoFar();
        }
        return node1->getPriority() > node2->getPriority();
    }

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


    void search(Way *startWay, Way *finalWay, Coordinates start, Coordinates end, NavigationGIS navigationGIS);


    void search(Way *startWay, Way *finalWay, Coordinates start, Coordinates destination, NavigationGIS navigationGIS,
                double (*heuristicFunc)(const Coordinates &start, const Coordinates &target),
                double (*costFunc)(Way &way), double (*comparator)(std::shared_ptr<Node>node1, std::shared_ptr<Node>node2));

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
