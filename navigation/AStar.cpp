#include "AStar.h"
#include "Route.h"
#include "../entities/Way.h"
#include "../NavigationGIS.h"

AStar::AStar(const NavigationGIS &navigationGis) : navigationGIS(navigationGis) {}

Route
AStar::shortestByDistance(const Way &startWay, const Way &finalWay, Coordinates start, Coordinates destination) {
    return searchShortestRoute(startWay, finalWay, start, destination, distanceHeuristic, costByDistance,
                               compareByDistance);
}

Route
AStar::shortestByTime(const Way &startWay, const Way &finalWay, Coordinates start, Coordinates destination) {
    return searchShortestRoute(startWay, finalWay, start, destination, timeHeuristic, costByTime, compareByTime);
}

Route
AStar::searchShortestRoute(const Way &startWay, const Way &finalWay, Coordinates start, Coordinates destination,
                           double (*heuristicFunc)(const Coordinates &start, const Coordinates &end),
                           double (*costFunc)(const Way &),
                           bool (*comparator)(std::shared_ptr<Node>, std::shared_ptr<Node>)) {


    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, std::function<bool(
            std::shared_ptr<Node>, std::shared_ptr<Node>)>> queue(comparator);
    Coordinates initialCoordinates = startWay.getToJunctionCoordinates(); // starting from to coordinates after we crossed the entire Way
    Meters initialDistance = startWay.getLength();
    Minutes initialTime = startWay.getTime();
    double initialCost = costFunc(startWay);
    double initialPriority = heuristicFunc(initialCoordinates, destination) + initialCost;
    std::shared_ptr<Node> startNode = std::make_shared<Node>(initialCoordinates, startWay.getToJunctionId(),
                                                             initialDistance, initialTime, initialCost, initialPriority,
                                                             Edge(std::make_pair(startWay.getId(), Direction::A_to_B)), nullptr);

    std::unordered_set<EntityId> popedJunctions; // we do lazy deletions so we need to keep track of which junction where already visited and poped from the queue
    queue.push(startNode);
    // if start way is bidirectional add another Node to the queue:
    if (startWay.isBidirectional()) {
        initialCoordinates = startWay.getFromJunctionCoordinates();
        initialPriority = heuristicFunc(initialCoordinates, destination) + initialCost;
        std::shared_ptr<Node> startNode2 = std::make_shared<Node>(initialCoordinates, startWay.getFromJunctionId(),
                                                                  initialDistance, initialTime, initialCost,
                                                                  initialPriority,
                                                                  Edge(std::make_pair(startWay.getId(), Direction::B_to_A)), nullptr);
        queue.push(startNode2);
    }

    std::shared_ptr<Node> currNode;
    while (!queue.empty()) {
        currNode = queue.top();
        queue.pop();
        if (popedJunctions.find(currNode->getJunctionId()) != popedJunctions.end()) {
            //      if node was already poped from the queue we can skip its copy
            continue;
        }
        popedJunctions.insert(currNode->getJunctionId());
        if (currNode->getJunctionId() == finalWay.getFromJunctionId() ||
            finalWay.isBidirectional() && currNode->getJunctionId() == finalWay.getToJunctionId()) {
            //        reached the final way :)
            Direction direction =
                    currNode->getJunctionId() == finalWay.getFromJunctionId() ? Direction::A_to_B : Direction::B_to_A;

            Coordinates finalCoordinates = direction == Direction::A_to_B ? finalWay.getToJunctionCoordinates()
                                                                          : finalWay.getFromJunctionCoordinates();
            EntityId finalJunctionId =
                    direction == Direction::A_to_B ? finalWay.getToJunctionId() : finalWay.getFromJunctionId();
            Meters distance = currNode->getDistanceSoFar() + finalWay.getLength();
            Minutes time = currNode->getTimeSoFar() + finalWay.getTime();
//            TODO delete this to lines because they are not relevant once reached destination.
            double cost = currNode->getCostSoFar() + costFunc(finalWay);
            double priority = cost;

            Edge finalEdge = std::make_pair(finalWay.getId(), direction);
            currNode = std::make_shared<Node>(finalCoordinates, finalJunctionId,
                                              distance, time, cost, priority,
                                              finalEdge, currNode);
            break;
        }

        std::vector<EntityId> wayEdgesIds = navigationGIS.getWaysByJunction(currNode->getJunctionId());

        for (auto wayId: wayEdgesIds) {
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

            queue.push(neighbor);
        }
    }
    if (currNode->getJunctionId() != finalWay.getToJunctionId() && currNode->getJunctionId() != finalWay.getFromJunctionId()) {
        // no Route was found, return invalid route
        return std::move(Route(start, destination));
    }

    std::vector<std::pair<EntityId, Direction>> ways = restoreShortestRoute(currNode);

    // if the way's direction is A_To_B we need to subtract the redundant distance from start point to from junction, else from to junction
    Meters trimFrontDistance = distanceFromWaysEnd(startWay, start, ways.front().second == Direction::A_to_B);
    // if the way's direction is A_To_B we need to subtract the redundant distance from destination point to from junction, else from to junction
    Meters trimTailDistance = distanceFromWaysEnd(finalWay, destination, ways.back().second == Direction::A_to_B);
    Meters distance = currNode->getDistanceSoFar() - trimFrontDistance - trimTailDistance;

    // if the way's direction is A_To_B we need to subtract the redundant time from start point to from junction, else from to junction
    Minutes trimFrontTime = timeFromWaysEnd(startWay, start, ways.front().second == Direction::A_to_B);
    // if the way's direction is A_To_B we need to subtract the redundant time from destination point to from junction, else from to junction
    Minutes trimTailTime = timeFromWaysEnd(finalWay, destination, ways.back().second == Direction::A_to_B);
    Minutes duration = currNode->getTimeSoFar() - trimFrontTime - trimTailTime;

    return std::move(Route(start, destination, distance, duration, ways, true));
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

double AStar::distanceHeuristic(const Coordinates &start, const Coordinates &target) {
    return (double) CoordinatesMath::calculateDistance(start, target);
}

double AStar::timeHeuristic(const Coordinates &start, const Coordinates &target) {
    // meters per minute
    return (double) CoordinatesMath::calculateDistance(start, target) / Way::kmh_to_mpm(MAX_SPEED);
}

double AStar::costByDistance(const Way &way) {
    return double(way.getLength());
}

double AStar::costByTime(const Way &way) {
    return (double) way.getTime();
}

bool AStar::compareByDistance(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
    if (node1->getPriority() == node2->getPriority()) {
        if (node1->getDistanceSoFar() == node2->getDistanceSoFar()) {
            return node1->getTimeSoFar() > node2->getTimeSoFar() ? true : false;
        }
        return node1->getDistanceSoFar() > node2->getDistanceSoFar();
    }
    return node1->getPriority() > node2->getPriority();
}

bool AStar::compareByTime(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
    if (node1->getPriority() == node2->getPriority()) {
        if (node1->getTimeSoFar() == node2->getTimeSoFar()) {
            return node1->getDistanceSoFar() > node2->getDistanceSoFar() ? true : false;
        }
        return node1->getTimeSoFar() > node2->getTimeSoFar();
    }
    return node1->getPriority() > node2->getPriority();
}

Meters AStar::distanceFromWaysEnd(const Way &way, Coordinates coordinates, bool from) {
    Coordinates endCoordinates = from ? way.getFromJunctionCoordinates() : way.getToJunctionCoordinates();
    return CoordinatesMath::calculateDistance(endCoordinates, coordinates);
}

Minutes AStar::timeFromWaysEnd(const Way &way, Coordinates coordinates, bool from) {
    Coordinates endCoordinates = from ? way.getFromJunctionCoordinates() : way.getToJunctionCoordinates();
    return Minutes((double) CoordinatesMath::calculateDistance(endCoordinates, coordinates) /
                   Way::kmh_to_mpm(way.getSpeedLimit()));
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

