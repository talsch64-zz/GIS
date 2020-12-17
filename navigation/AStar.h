

#ifndef GIS_CPP_ASTAR_H
#define GIS_CPP_ASTAR_H

#include <queue>
#include <unordered_map>
#include <functional>
#include <memory>
#include "../GISNamedTypes.h"
#include "../entities/Way.h"
#include "../NavigationGIS.h"

class AStar {

public:
    class Node {
        Coordinates coordinates;
        EntityId junctionId;
        double g_n;
        // f_n = g(n) + h(n) where h is the heuristic function
        double f_n;
        std::optional<std::pair<EntityId, Direction>> prevEdgeWay;

    public:
        double getCost();


        Node(const Coordinates &coordinates, const EntityId &junctionId, double g_n, std::optional<std::pair<EntityId, Direction>> prevEdgeWay);

        const Coordinates &getCoordinates() const;

        void setGN(double gN);

        void setFN(double fN);

        const EntityId &getJunctionId() const;

        bool operator<(const Node &other) {
            return f_n > other.f_n;
        }

        std::vector<std::shared_ptr<AStar::Node>> getNeighbors(std::shared_ptr<Node> currNode, std::unordered_map<EntityId, std::shared_ptr<Node>, NavigationGIS)

    };

    double heuristic(const Coordinates &start, const Coordinates &target);

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
