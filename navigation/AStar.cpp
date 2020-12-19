#include <iostream>
#include "AStar.h"
#include "Route.h"
#include "../entities/Way.h"
#include "../NavigationGIS.h"


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

    bool firstJunctionFound = false;
/*-------------------------------- initialize initial Nodes --------------------------------*/
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, std::function<bool(
            std::shared_ptr<Node>, std::shared_ptr<Node>)>> queue(comparator);

    queue.push(createInitialNode(heuristicFunc, costFunc, Direction::A_to_B));
    // if start way is bidirectional add another Node to the queue:
    if (startWay.isBidirectional()) {
        queue.push(createInitialNode(heuristicFunc, costFunc, Direction::B_to_A));
    }

    std::unordered_set<EntityId> popedJunctions; // we do lazy deletions so we need to keep track of which junction where already visited and poped from the queue
    std::shared_ptr<Node> currNode;

/*-------------------------------- performing A* algorithm with the queue --------------------------------*/
    while (!queue.empty()) {
        currNode = queue.top();
        queue.pop();
        if (popedJunctions.find(currNode->getJunctionId()) != popedJunctions.end()) {
            //  if node was already poped from the queue we can skip its copy
            continue;
        }
        popedJunctions.insert(currNode->getJunctionId());

        //  reached the final way and initializing the final Node
        if (currNode->getJunctionId() == finalWay.getFromJunctionId() ||
            finalWay.isBidirectional() && currNode->getJunctionId() == finalWay.getToJunctionId()) {
            currNode = createNeighbor(currNode, finalWay.getId(), heuristicFunc, costFunc);
            break;
        }

        std::vector<EntityId> wayEdgesIds = navigationGIS.getWaysByJunction(currNode->getJunctionId());
        for (auto wayId: wayEdgesIds) {  // visit all the neighbors and add them to the queue
            std::shared_ptr<Node> neighbor = createNeighbor(currNode, wayId, heuristicFunc, costFunc);
            queue.push(neighbor);
        }
    }
    if (currNode->getJunctionId() != finalWay.getToJunctionId() &&
        currNode->getJunctionId() != finalWay.getFromJunctionId()) {
        // no Route was found, return invalid route
        return std::move(Route(origin, destination));
    }

    std::vector<std::pair<EntityId, Direction>> ways = restoreShortestRoute(currNode);

    /* if the way's direction is A_To_B we need to subtract the redundant distance and time from start point to "from" junction, else from "to" junction
     if the way's direction is A_To_B we need to subtract the redundant distance and time from destination point to "to" junction, else from "from" junction */

    Meters redundantFrontDistance = distanceFromWaysEnd(startWay, origin, ways.front().second == Direction::A_to_B);
    Meters redundantTailDistance = distanceFromWaysEnd(finalWay, destination, ways.back().second == Direction::B_to_A);
    Meters distance = currNode->getDistanceSoFar() - redundantFrontDistance - redundantTailDistance;

    Minutes trimFrontTime = timeFromWaysEnd(startWay, origin, ways.front().second == Direction::A_to_B);
    Minutes trimTailTime = timeFromWaysEnd(finalWay, destination, ways.back().second == Direction::B_to_A);
    Minutes duration = currNode->getTimeSoFar() - trimFrontTime - trimTailTime;

    return std::move(Route(origin, destination, distance, duration, ways, true));
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
    return (double) CoordinatesMath::calculateDistance(coordinates, destination) / Way::kmh_to_mm(MAX_SPEED);
}

double AStar::costByDistance(const Way &way) {
    return (double)way.getLength();
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

Meters AStar::distanceFromWaysEnd(const Way &way, Coordinates coordinates, bool front) {
    Coordinates endCoordinates = front ? way.getFromJunctionCoordinates() : way.getToJunctionCoordinates();
    return CoordinatesMath::calculateDistance(endCoordinates, coordinates);
}

Minutes AStar::timeFromWaysEnd(const Way &way, Coordinates coordinates, bool front) {
    Coordinates endCoordinates = front ? way.getFromJunctionCoordinates() : way.getToJunctionCoordinates();
    return Minutes((double) CoordinatesMath::calculateDistance(endCoordinates, coordinates) /
                           Way::kmh_to_mm(way.getSpeedLimit()));
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
    EntityId initialJunctionId =
            direction == Direction::A_to_B ? startWay.getToJunctionId() : startWay.getFromJunctionId();
    Meters initialDistance = startWay.getLength();
    Minutes initialTime = startWay.getTime();
    double initialCost = costFunc(startWay);
    double initialPriority = heuristicFunc(initialCoordinates, destination) + initialCost;
    std::shared_ptr<Node> initialNode = std::make_shared<Node>(initialCoordinates, initialJunctionId,
                                                               initialDistance, initialTime, initialCost,
                                                               initialPriority,
                                                               Edge(std::make_pair(startWay.getId(),
                                                                                   direction)),
                                                               nullptr);
    return initialNode;
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

