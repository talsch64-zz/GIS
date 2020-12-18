#include "AStar.h"
#include "Route.h"
#include "../entities/Way.h"
#include "../NavigationGIS.h"

AStar::AStar(const NavigationGIS &navigationGis) : navigationGIS(navigationGis) {}

const Route &AStar::shortestByDistance(Way *startWay, Way *finalWay, Coordinates start, Coordinates destination) {
    return search(startWay, finalWay, start, destination, distanceHeuristic, costByDistance, compareByDistance);
}

const Route &AStar::shortestByTime(Way *startWay, Way *finalWay, Coordinates start, Coordinates destination) {
    return search(startWay, finalWay, start, destination, timeHeuristic, costByTime, compareByTime);
}

const Route &
AStar::search(Way *startWay, Way *finalWay, Coordinates start, Coordinates destination,
              double (*heuristicFunc)(const Coordinates &start, const Coordinates &end),
              double (*costFunc)(const Way &),
              bool (*comparator)(std::shared_ptr<Node>, std::shared_ptr<Node>)) {


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

double AStar::distanceHeuristic(const Coordinates &start, const Coordinates &target) {
    return (double) CoordinatesMath::calculateDistance(start, target);
}

double AStar::timeHeuristic(const Coordinates &start, const Coordinates &target) {
    // meters per hour
    return (double) CoordinatesMath::calculateDistance(start, target) / kmh_to_mh(MAX_SPEED);
}

double AStar::costByDistance(const Way &way) {
    return (double) CoordinatesMath::calculateDistance(way.getFromJunctionCoordinates(),
                                                       way.getToJunctionCoordinates());
}

double AStar::costByTime(const Way &way) {
    return (double) way.getLength() / kmh_to_mh(way.getSpeedLimit());
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

