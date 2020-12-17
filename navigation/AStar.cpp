#include "AStar.h"
#include "../entities/Way.h"
#include "../NavigationGIS.h"

void AStar::search(Way *startWay, Way *finalWay, Coordinates start, Coordinates end, NavigationGIS navigationGIS) {
//    TODO add heuristics as parameter
    std::unordered_map<EntityId, std::shared_ptr<AStar::Node>> nodes;
    std::priority_queue<std::shared_ptr<AStar::Node>> queue;
    std::shared_ptr<AStar::Node> startNode = std::make_shared<AStar::Node>(startWay->getToJunctionCoordinates(),
                                                                           startWay->getTo(), 0, nullptr);
    startNode->setFN(heuristic(startWay->getToJunctionCoordinates(), end));
    queue.push(startNode);


    while (!queue.empty()) {
        std::shared_ptr<Node> currNode = queue.top();
        queue.pop();
//        TODO handle bidirectional finalWay
        if (currNode->getJunctionId() == finalWay->getFrom()) {
            break;
        }

        std::vector<EntityId> wayEdges = navigationGIS.getWaysByJunction(currNode->getJunctionId());
        std::vector<std::shared_ptr<AStar::Node>> neighbors;


        graph.get_neighbors(current, neighbors);
        for (Location next : neighbors) {
            cost_t new_cost = cost_so_far[current] + graph.cost(current, next);
            if (cost_so_far.find(next) == cost_so_far.end()
                || new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                cost_t priority = new_cost + heuristic(next, goal);
                frontier.put(next, priority);
                came_from[next] = current;
            }
        }
    }
}

AStar::Node::Node(const Coordinates &coordinates, const EntityId &junctionId, double g_n, std::optional<std::pair<EntityId, Direction>> prevEdgeWay) : coordinates(coordinates),
                                                                                            junctionId(junctionId),
                                                                                            g_n(g_n), prevEdgeWay(std::move(prevEdgeWay)) {}

const Coordinates &AStar::Node::getCoordinates() const {
    return coordinates;
}

const EntityId &AStar::Node::getJunctionId() const {
    return junctionId;
}

void AStar::Node::setGN(double gN) {
    g_n = gN;
}

void AStar::Node::setFN(double fN) {
    f_n = fN;
}



