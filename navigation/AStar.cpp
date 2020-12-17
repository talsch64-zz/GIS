#include "AStar.h"
#include "../entities/Way.h"
#include "../NavigationGIS.h"

void AStar::search(Way *startWay, Way *finalWay, Coordinates start, Coordinates end, NavigationGIS navigationGIS) {
//    TODO add heuristics as parameter
    std::unordered_map<EntityId, std::shared_ptr<AStar::Node>> nodes;
    std::priority_queue<std::shared_ptr<AStar::Node>> queue;
    std::shared_ptr<AStar::Node> startNode = std::make_shared<AStar::Node>(startWay->getToJunctionCoordinates(),
                                                                           startWay->getTo(), 0,
                                                                           std::optional<std::pair<EntityId, Direction>>());
    startNode->setPriority(heuristic(startWay->getToJunctionCoordinates(), end));
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

        for (auto wayId: wayEdges) {
            Way &way = navigationGIS.getWay(wayId);
            EntityId neighborId = way.getFrom() == currNode->getJunctionId() ? way.getTo() : way.getFrom();
            Coordinates neighborCoordinates =
                    way.getFromJunctionCoordinates() == currNode->getCoordinates() ? way.getToJunctionCoordinates()
                                                                                   : way.getFromJunctionCoordinates();
            auto findNeighbor = nodes.find(neighborId);
            std::shared_ptr<AStar::Node> neighbor;
            double newCost = (double) way.getLength() + currNode->getCost();
            if (findNeighbor != nodes.end()) {
                neighbor = findNeighbor->second;
                if (neighbor->getCost() > newCost) {
                    neighbor->setCost(newCost);
                }
            } else {
                Direction direction = way.getFrom() == currNode->getJunctionId() ? Direction::A_to_B : Direction::B_to_A;
                neighbor = std::make_shared<AStar::Node>(neighborCoordinates, neighborId,
                                                         newCost, std::make_pair(way.getId(), direction));
            }
            neighbor->setPriority(heuristic(neighborCoordinates, end));
        }

    }
}

AStar::Node::Node(const Coordinates &coordinates, const EntityId &junctionId, double g_n,
                  std::optional<std::pair<EntityId, Direction>> prevEdgeWay) : coordinates(coordinates),
                                                                               junctionId(junctionId),
                                                                               cost(g_n),
                                                                               prevEdgeWay(std::move(prevEdgeWay)) {}

const Coordinates &AStar::Node::getCoordinates() const {
    return coordinates;
}

const EntityId &AStar::Node::getJunctionId() const {
    return junctionId;
}

void AStar::Node::setCost(double _cost) {
    cost = _cost;
}

void AStar::Node::setPriority(double _priority) {
    priority = _priority;
}

double AStar::Node::getCost() const {
    return cost;
}

const std::optional<std::pair<EntityId, Direction>> &AStar::Node::getPrevEdgeWay() const {
    return prevEdgeWay;
}



//std::vector<std::shared_ptr<AStar::Node>>
//AStar::Node::getNeighbors(std::shared_ptr<Node> currNode, std::unordered_map<EntityId, std::shared_ptr<Node>>,
//                          NavigationGIS &navigationGIS) {
//    std::vector<EntityId> wayEdges = navigationGIS.getWaysByJunction(currNode->getJunctionId());
//
//    return std::vector<std::shared_ptr<AStar::Node>>();
//}



