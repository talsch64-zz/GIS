#include "AStar.h"
#include "Route.h"
#include "../entities/Way.h"
#include "../NavigationGIS.h"


const Route &AStar::search(Way *startWay, Way *finalWay, Coordinates start, Coordinates destination, NavigationGIS navigationGIS,
              double (*heuristicFunc)(const Coordinates &start, const Coordinates &end), double (*costFunc)(const Way &),
              double (*comparator)(std::shared_ptr<Node>, std::shared_ptr<Node>)) {


    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, std::function<bool(
            std::shared_ptr<Node>, std::shared_ptr<Node>)>> queue(comparator);
    Coordinates initialCoordinate = startWay->getToJunctionCoordinates(); // starting from to coordinates after we crossed the entire Way
    Meters initialDistance(startWay->getLength());
    Minutes initialTime((double) startWay->getLength() / kmh_to_mh(startWay->getSpeedLimit()));
    double initialCost = costFunc(*startWay);
    double initialPriority = heuristicFunc(startWay->getToJunctionCoordinates(), destination) + initialCost;
    std::shared_ptr<Node> startNode = std::make_shared<Node>(initialCoordinate, startWay->getToJunctionId(),
                                                             initialDistance, initialTime, initialCost, initialPriority,
                                                             Edge());

    std::unordered_set<EntityId> popedJunctions;

    queue.push(startNode);
    std::shared_ptr<Node> currNode;

    while (!queue.empty()) {
        currNode = queue.top();
        queue.pop();
//        TODO handle bidirectional finalWay
        if (popedJunctions.find(currNode->getJunctionId()) != popedJunctions.end()) {
            //      if node was already poped from the queue we can skip its copy
            continue;
        }
//        reached the destination :)
        if (currNode->getJunctionId() == finalWay->getFromJunctionId()) {
            break;
        }

        std::vector<EntityId> wayEdges = navigationGIS.getWaysByJunction(currNode->getJunctionId());
        std::vector<std::shared_ptr<Node>> neighbors;

        for (auto wayId: wayEdges) {
            const Way &way = navigationGIS.getWay(wayId);
            EntityId neighborId = way.getFromJunctionId() == currNode->getJunctionId() ? way.getToJunctionId()
                                                                                       : way.getFromJunctionId();
            Coordinates neighborCoordinates =
                    way.getFromJunctionCoordinates() == currNode->getCoordinates() ? way.getToJunctionCoordinates()
                                                                                   : way.getFromJunctionCoordinates();

            Meters distanceSoFar = currNode->getDistanceSoFar() + way.getLength();
            Minutes timeSoFar = currNode->getTimeSoFar() + Minutes(way.getLength() / kmh_to_mh(way.getSpeedLimit()));
            double costSoFar = costFunc(way) + currNode->getCostSoFar();
            double priority = heuristicFunc(neighborCoordinates, destination) + costSoFar;
            Direction direction =
                    way.getFromJunctionId() == currNode->getJunctionId() ? Direction::A_to_B : Direction::B_to_A;
            std::shared_ptr<Node> neighbor = std::make_shared<Node>(neighborCoordinates, neighborId, distanceSoFar,
                                                                    timeSoFar, costSoFar, priority,
                                                                    std::make_pair(wayId, direction));

            queue.push(neighbor);
        }
    }
    if (currNode->getJunctionId() != finalWay->getToJunctionId()) {
        // no Route was found, return invalid route
        return std::move(Route(start, destination));
    }
    Minutes duration = currNode->getTimeSoFar();
    Meters distance = currNode->getDistanceSoFar() -
                      CoordinatesMath::calculateDistance(start, startWay->getFromJunctionCoordinates()) -
                      CoordinatesMath::calculateDistance(destination, finalWay->getToJunctionCoordinates());
    std::vector<std::pair<EntityId, Direction>> ways = restoreShortestRoute(currNode);
    return std::move(Route(start, destination, distance, duration, ways, true));
}

std::vector<std::pair<EntityId, Direction>> AStar::restoreShortestRoute(std::shared_ptr<Node> node) {
    std::vector<std::pair<EntityId, Direction>> ways;
    while (node->getPrevEdgeWay().has_value()) {
        ways.push_back(node->getPrevEdgeWay().value());
    }
    std::reverse(ways.begin(), ways.end());
    return ways;
}


AStar::Node::Node(const Coordinates &coordinates, const EntityId &junctionId,
                  const Meters &distanceSoFar, const Minutes &timeSoFar, double costSoFar, double priority,
                  const Edge &prevEdgeWay) : coordinates(coordinates),
                                             junctionId(junctionId),
                                             distanceSoFar(distanceSoFar),
                                             timeSoFar(timeSoFar),
                                             costSoFar(costSoFar),
                                             priority(priority),
                                             prevEdgeWay(prevEdgeWay) {}


const Coordinates &AStar::Node::getCoordinates() const {
    return coordinates;
}

const EntityId &AStar::Node::getJunctionId() const {
    return junctionId;
}


const AStar::Edge &AStar::Node::getPrevEdgeWay() const {
    return prevEdgeWay;
}

double AStar::Node::getPriority() const {
    return priority;
}

const Meters &AStar::Node::getDistanceSoFar() const {
    return distanceSoFar;
}

const Minutes &AStar::Node::getTimeSoFar() const {
    return timeSoFar;
}

double AStar::Node::getCostSoFar() const {
    return costSoFar;
}



//TODO delete function
//void AStar::search(Way *startWay, Way *finalWay, Coordinates start, Coordinates end, NavigationGIS navigationGIS) {
////    TODO add heuristics as parameter
//    std::unordered_map<EntityId, std::shared_ptr<AStar::Node>> nodes;
//    std::priority_queue<std::shared_ptr<AStar::Node>> queue;
//    std::shared_ptr<AStar::Node> startNode = std::make_shared<AStar::Node>(startWay->getToJunctionCoordinates(),
//                                                                           startWay->getToJunctionId(), 0,
//                                                                           std::optional<std::pair<EntityId, Direction>>());
//    startNode->setPriority(heuristic(startWay->getToJunctionCoordinates(), end));
//    queue.push(startNode);
//
//
//    while (!queue.empty()) {
//        std::shared_ptr<Node> currNode = queue.top();
//        queue.pop();
////        TODO handle bidirectional finalWay
//        if (currNode->getJunctionId() == finalWay->getFromJunctionId()) {
//            break;
//        }
//
//        std::vector<EntityId> wayEdges = navigationGIS.getWaysByJunction(currNode->getJunctionId());
//        std::vector<std::shared_ptr<AStar::Node>> neighbors;
//
//        for (auto wayId: wayEdges) {
//            Way &way = navigationGIS.getWay(wayId);
//            EntityId neighborId = way.getFromJunctionId() == currNode->getJunctionId() ? way.getToJunctionId() : way.getFromJunctionId();
//            Coordinates neighborCoordinates =
//                    way.getFromJunctionCoordinates() == currNode->getCoordinates() ? way.getToJunctionCoordinates()
//                                                                                   : way.getFromJunctionCoordinates();
//            auto findNeighbor = nodes.find(neighborId);
//            std::shared_ptr<AStar::Node> neighbor;
//            double newCost = (double) way.getLength() + currNode->getCost();
//            if (findNeighbor != nodes.end()) {
//                neighbor = findNeighbor->second;
//                if (neighbor->getCost() > newCost) {
//                    neighbor->setCost(newCost);
//                }
//            } else {
//                Direction direction =
//                        way.getFromJunctionId() == currNode->getJunctionId() ? Direction::A_to_B : Direction::B_to_A;
//                neighbor = std::make_shared<AStar::Node>(neighborCoordinates, neighborId,
//                                                         newCost, std::make_pair(way.getId(), direction));
//            }
//            neighbor->setPriority(heuristic(neighborCoordinates, end));
//        }
//
//    }
//}