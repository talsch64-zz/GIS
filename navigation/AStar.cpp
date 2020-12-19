#include <iostream>
#include "AStar.h"
#include "Route.h"
#include "../entities/Way.h"
#include "../NavigationGIS.h"
#include "../Utils.h"


AStar::AStar(const NavigationGIS &navigationGis, const Coordinates &origin, const Coordinates &destination,
             const Way &startWay, const Way &finalWay) : navigationGIS(navigationGis), origin(origin),
                                                         destination(destination), startWay(startWay),
                                                         finalWay(finalWay) {}

Route
AStar::shortestByDistance() {
    return searchShortestRoute(distanceHeuristic, costByDistance, compareByDistance);
}

Route
AStar::shortestByTime() {
    return searchShortestRoute(timeHeuristic, costByTime, compareByTime);
}

Route
AStar::searchShortestRoute(double (*heuristicFunc)(const Coordinates &start, const Coordinates &end),
                           double (*costFunc)(const Way &),
                           bool (*comparator)(std::shared_ptr<Node>, std::shared_ptr<Node>)) {

/*-------------------------------- initialize initial Nodes --------------------------------*/
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, std::function<bool(
            std::shared_ptr<Node>, std::shared_ptr<Node>)>> queue(comparator);

    queue.push(createInitialNode(heuristicFunc, costFunc, Direction::A_to_B));
    // if start way is bidirectional add another Node to the queue:
    if (startWay.isBidirectional()) {
        queue.push(createInitialNode(heuristicFunc, costFunc, Direction::B_to_A));
    }
    // we do lazy deletions so we need to keep track of which junctions where already visited and poped from the queue
    std::unordered_set<EntityId> popedJunctions;
    std::shared_ptr<Node> currNode;

/*-------------------------------- performing A* algorithm with the queue --------------------------------*/
    while (!queue.empty()) {
        currNode = queue.top();
        queue.pop();

        if (currNode->getJunctionId() == EntityId("")) {   // reached destination!!!
            break;
        }

        if (popedJunctions.find(currNode->getJunctionId()) != popedJunctions.end()) {
            //  if node was already poped from the queue we can skip its copy, lazy deletions
            continue;
        }
        popedJunctions.insert(currNode->getJunctionId());

        //  reached the final way and initializing the final Node
        if (currNode->getJunctionId() == finalWay.getFromJunctionId() ||
            finalWay.isBidirectional() && currNode->getJunctionId() == finalWay.getToJunctionId()) {
            queue.push(createFinalNode(currNode, heuristicFunc, costFunc));
        }

        std::vector<EntityId> wayEdgesIds = navigationGIS.getWaysByJunction(currNode->getJunctionId());
        for (auto wayId: wayEdgesIds) {  // visit all the neighbors and add them to the queue
            std::shared_ptr<Node> neighbor = createNeighbor(currNode, wayId, heuristicFunc, costFunc);
            queue.push(neighbor);
        }
    }
/*-------------------------------- After search finished --------------------------------*/
//TODO maybe make EntityId optional field, more elegant
    if (currNode->getJunctionId() != EntityId("")) {
        // no Route was found, return invalid route. Empty entityId means currNode is a final Node represents destination point
        return Route::invalidRoute(origin, destination);
    }

    std::vector<std::pair<EntityId, Direction>> ways = restoreShortestRoute(currNode);
    return std::move(Route(origin, destination, currNode->getDistanceSoFar(), currNode->getTimeSoFar(), ways, true));
}

std::vector<std::pair<EntityId, Direction>> AStar::restoreShortestRoute(std::shared_ptr<Node> node) {
    std::vector<std::pair<EntityId, Direction>> ways;
    // backtrack the route until the first node is reached
    while (node != nullptr) {
        ways.push_back(node->getPrevEdgeWay().value());
        node = node->getPrevNode();
    }
    std::reverse(ways.begin(), ways.end());
    return ways;
}

double AStar::distanceHeuristic(const Coordinates &coordinates, const Coordinates &destination) {
    return (double) CoordinatesMath::calculateDistance(coordinates, destination);
}

double AStar::timeHeuristic(const Coordinates &coordinates, const Coordinates &destination) {
    // meters per minute
    return (double) calculateTime(CoordinatesMath::calculateDistance(coordinates, destination), MAX_SPEED);
}

double AStar::costByDistance(const Way &way) {
    return (double) way.getLength();
}

double AStar::costByTime(const Way &way) {
    return (double) way.getTime();
}

bool AStar::compareByDistance(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
    if (node1->getPriority() == node2->getPriority()) {
        return node1->getTimeSoFar() > node2->getTimeSoFar();
    }
    return node1->getPriority() > node2->getPriority();
}

bool AStar::compareByTime(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
    if (node1->getPriority() == node2->getPriority()) {
        return node1->getDistanceSoFar() > node2->getDistanceSoFar();
    }
    return node1->getPriority() > node2->getPriority();
}

std::shared_ptr<AStar::Node> AStar::createNeighbor(std::shared_ptr<Node> currNode, EntityId wayId,
                                                   double (*heuristicFunc)(const Coordinates &, const Coordinates &),
                                                   double (*costFunc)(const Way &)) {
    const Way &way = navigationGIS.getWay(wayId);
    Direction direction =
            way.getFromJunctionId() == currNode->getJunctionId() ? Direction::A_to_B : Direction::B_to_A;
    EntityId neighborId = direction == Direction::A_to_B ? way.getToJunctionId() : way.getFromJunctionId();
    Coordinates neighborCoordinates = direction == Direction::A_to_B ? way.getToJunctionCoordinates()
                                                                     : way.getFromJunctionCoordinates();
    Meters distanceSoFar = currNode->getDistanceSoFar() + way.getLength();
    Minutes timeSoFar = currNode->getTimeSoFar() + way.getTime();
    double costSoFar = currNode->getCostSoFar() + costFunc(way);
    double priority = costSoFar + heuristicFunc(neighborCoordinates, destination);

    std::shared_ptr<Node> neighbor = std::make_shared<Node>(neighborCoordinates, neighborId, distanceSoFar,
                                                            timeSoFar, costSoFar, priority,
                                                            std::make_pair(wayId, direction), currNode);
    return neighbor;
}

std::shared_ptr<AStar::Node> AStar::createInitialNode(double (*heuristicFunc)(const Coordinates &, const Coordinates &),
                                                      double (*costFunc)(const Way &), Direction direction) {
    //  If direction is A_To_B then the Node start at "to" junction, else from "from" junction.
    //  We implemented the algorithm such that the initial Node already has a "kilometrage" of the startWay
    Coordinates initialCoordinates = direction == Direction::A_to_B ? startWay.getToJunctionCoordinates()
                                                                    : startWay.getFromJunctionCoordinates();
    Coordinates oppositeCoordinates = direction == Direction::A_to_B ? startWay.getFromJunctionCoordinates()
                                                                     : startWay.getToJunctionCoordinates();
    //  the distance from origin point to the initial node
    Meters initialDistance =
            startWay.getLength() - CoordinatesMath::calculateDistance(oppositeCoordinates, origin);
    std::cout << "distance from junction to origin:  "<< (double) initialDistance << std::endl;

    //  the time from origin point to the initial node
    Minutes initialTime = calculateTime(initialDistance, startWay.getSpeedLimit());
    EntityId initialJunctionId =
            direction == Direction::A_to_B ? startWay.getToJunctionId() : startWay.getFromJunctionId();
//    TODO find a better solution to update the initial cost
    double initialCost = costFunc == costByTime ? double(initialTime) : double(initialDistance);
    double initialPriority = heuristicFunc(initialCoordinates, destination) + initialCost;
    std::shared_ptr<Node> initialNode = std::make_shared<Node>(initialCoordinates, initialJunctionId,
                                                               initialDistance, initialTime, initialCost,
                                                               initialPriority,
                                                               Edge(std::make_pair(startWay.getId(),
                                                                                   direction)),
                                                               nullptr);
    return initialNode;
}

std::shared_ptr<AStar::Node> AStar::createFinalNode(std::shared_ptr<Node> currNode,
                                                    double (*heuristicFunc)(const Coordinates &, const Coordinates &),
                                                    double (*costFunc)(const Way &)
) {
    Direction direction =
            finalWay.getFromJunctionId() == currNode->getJunctionId() ? Direction::A_to_B : Direction::B_to_A;
    // if direction is A_to_B we want to trim the aerial distance from "to" Junction to destination point, else from "from" Junction
    Coordinates oppositeCoordinates = direction == Direction::A_to_B ? finalWay.getToJunctionCoordinates()
                                                                     : finalWay.getFromJunctionCoordinates();
    // currNode represents the opposite junction of endCoordinates
    Meters distanceFromCurrNode =
            finalWay.getLength() - CoordinatesMath::calculateDistance(oppositeCoordinates, destination);
    std::cout << "distance from junction to destination:  "<< (double) distanceFromCurrNode << std::endl;
    //  the total time to reach destination point
    Minutes finalTime = calculateTime(distanceFromCurrNode, finalWay.getSpeedLimit()) + currNode->getTimeSoFar();
    //  the total distance to reach destination point
    Meters finalDistance = distanceFromCurrNode + currNode->getDistanceSoFar();
    double finalCost = costFunc == costByTime ? (double) finalTime : (double) finalDistance;
    double finalPriority = finalCost; // reached destination so priority = cost :)
    std::shared_ptr<Node> finalNode = std::make_shared<Node>(destination, EntityId(""),
                                                             finalDistance, finalTime, finalCost,
                                                             finalPriority,
                                                             Edge(std::make_pair(finalWay.getId(),
                                                                                 direction)),
                                                             currNode);
    return finalNode;
}


AStar::Node::Node(const Coordinates &coordinates, const EntityId &junctionId,
                  const Meters &distanceSoFar, const Minutes &timeSoFar, double costSoFar, double priority,
                  const Edge &prevEdgeWay, std::shared_ptr<Node> prevNode) : coordinates(coordinates),
                                                                             junctionId(junctionId),
                                                                             distanceSoFar(distanceSoFar),
                                                                             timeSoFar(timeSoFar),
                                                                             costSoFar(costSoFar),
                                                                             priority(priority),
                                                                             prevEdgeWay(prevEdgeWay),
                                                                             prevNode(prevNode) {}


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

const std::shared_ptr<AStar::Node> &AStar::Node::getPrevNode() const {
    return prevNode;
}

