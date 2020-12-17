

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
class AStar {

public:
    class Node {
        Coordinates coordinates;
        EntityId junctionId;
        double cost;
        // priority(n) = cost(n) + h(n) where h is the heuristic function
        double priority;
        std::optional<std::pair<EntityId, Direction>> prevEdgeWay;

    public:


        Node(const Coordinates &coordinates, const EntityId &junctionId, double g_n, std::optional<std::pair<EntityId, Direction>> prevEdgeWay);

        double getCost() const;

        const std::optional<std::pair<EntityId, Direction>> &getPrevEdgeWay() const;

        const Coordinates &getCoordinates() const;

        void setCost(double _cost);

        void setPriority(double _priority);

        const EntityId &getJunctionId() const;

        bool operator<(const Node &other) {
            return priority > other.priority;
        }

        std::vector<std::shared_ptr<AStar::Node>> getNeighbors(std::shared_ptr<Node> currNode, std::unordered_map<EntityId, std::shared_ptr<Node>>, NavigationGIS &gis);

    };

    double heuristic(const Coordinates &start, const Coordinates &target) {return (double) CoordinatesMath::calculateDistance(start, target);}

    void search(Way *startWay, Way *finalWay, Coordinates start, Coordinates end, NavigationGIS navigationGIS);
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
