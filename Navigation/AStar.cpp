#include "AStar.h"
#include "Route.h"
#include "../UserCommon/Utils.h"


AStar::AStar(const NavigationGIS &navigationGis, const Coordinates &origin, const Coordinates &destination,
             const AbstractWay &startWay, const std::size_t startWaySegment, const AbstractWay &finalWay,
             const std::size_t finalWaySegment) : navigationGIS(navigationGis), origin(origin),
                                             destination(destination), startWay(startWay),
                                             startWaySegment(startWaySegment),
                                             finalWay(finalWay), finalWaySegment(finalWaySegment),
                                             restrictions(Restrictions("")) {}


AStar::AStar(const NavigationGIS &navigationGis, const Coordinates &origin, const Coordinates &destination,
             const AbstractWay &startWay, const std::size_t startWaySegment, const AbstractWay &finalWay,
             const std::size_t finalWaySegment, const Restrictions &restrictions) :

        navigationGIS(navigationGis),
        origin(origin),
        destination(destination),
        startWay(startWay),
        startWaySegment(startWaySegment),
        finalWay(finalWay),
        finalWaySegment(finalWaySegment),
        restrictions(restrictions) {}


std::unique_ptr<Route>
AStar::shortestByDistance() {
    std::optional<std::unique_ptr<Route>> singleWayRoute;
    if (startWay.getId() == finalWay.getId()) {
        singleWayRoute = getSingleWayRoute();
    }
    std::unique_ptr<Route> shortestRoute = searchShortestRoute(distanceHeuristic, costByDistance, compareByDistance);
    if (!singleWayRoute.has_value()) {
        // the route is not single route so just return the route the A* algorithm found
        return shortestRoute;
    }
    if (shortestRoute == nullptr) {
        // it is possible that A* didn't find a route if the route is single-way route
        return std::move(singleWayRoute.value());
    }

    if (shortestRoute->totalLength() < singleWayRoute.value()->totalLength()) {
        // The shortestRoute is indeed the shortest
        return shortestRoute;
    }
    if (shortestRoute->totalLength() == singleWayRoute.value()->totalLength() &&
        shortestRoute->estimatedDuration() < singleWayRoute.value()->estimatedDuration()) {
        // both route are the same length but shortestRoute is faster
        return shortestRoute;
    }

    return std::move(singleWayRoute.value());
}

std::unique_ptr<Route>
AStar::shortestByTime() {
    std::optional<std::unique_ptr<Route>> singleWayRoute;
    if (startWay.getId() == finalWay.getId()) {
        singleWayRoute = getSingleWayRoute();
    }
    std::unique_ptr<Route> fastestRoute = searchShortestRoute(timeHeuristic, costByTime, compareByTime);
    if (!singleWayRoute.has_value()) {
        // the route is not single route so just return the route the A* algorithm found
        return fastestRoute;
    }
    if (fastestRoute == nullptr) {
        // it is possible that A* didn't find a route if the route is single-way route
        return std::move(singleWayRoute.value());
    }

    if (fastestRoute->estimatedDuration() < singleWayRoute.value()->estimatedDuration()) {
        // fastestRoute is indeed the fastest
        return fastestRoute;
    }
    if (fastestRoute->estimatedDuration() == singleWayRoute.value()->estimatedDuration() &&
        fastestRoute->totalLength() < singleWayRoute.value()->totalLength()) {
        // both route are equal in duration but fastestRoute is shorter in length
        return fastestRoute;
    }

    return std::move(singleWayRoute.value());
}

std::unique_ptr<Route>
AStar::searchShortestRoute(double (*heuristicFunc)(const Coordinates &start, const Coordinates &end),
                           double (*costFunc)(const AbstractWay &),
                           bool (*comparator)(std::shared_ptr<Node>, std::shared_ptr<Node>)) {
    auto idPair = finalWay.getJunctions();
    auto finalWayFromId = idPair.first;
    auto finalWayToId = idPair.second;

/*-------------------------------- initialize initial Nodes --------------------------------*/
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, std::function<bool(
            std::shared_ptr<Node>, std::shared_ptr<Node>)>> queue(comparator);

// For lazy deletions efficiency - each node inserted to the queue only if its priority <= the curr priority of the node (represented by its junction id)
// This map is not a mandatory, it just increases efficiency (redundant nodes are not added to the queue)
    std::unordered_map<EntityId, double> minNodes;
    std::unordered_map<EntityId, const AbstractWay&> waysMap;

    std::shared_ptr<Node> initialNode = createInitialNode(heuristicFunc, costFunc, Direction::A_to_B);
    queue.push(initialNode);
    minNodes.insert(std::pair<EntityId, double>(initialNode->getJunctionId(), initialNode->getPriority()));
    waysMap.insert(std::pair<EntityId, const AbstractWay&>(startWay.getId(), startWay)); // lowers gis usage


    // if start way is bidirectional add another Node to the queue:
    if (startWay.isBidirectional()) {
        initialNode = createInitialNode(heuristicFunc, costFunc, Direction::B_to_A);
        queue.push(initialNode);
        minNodes.insert(std::pair<EntityId, double>(initialNode->getJunctionId(), initialNode->getPriority()));
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
        if (currNode->getJunctionId() == finalWayFromId ||
            (finalWay.isBidirectional() && currNode->getJunctionId() == finalWayToId)) {
            queue.push(createFinalNode(currNode, costFunc));
        }

        std::vector<EntityId> wayEdgesIds = navigationGIS.getWaysByJunction(currNode->getJunctionId());
        for (auto wayId: wayEdgesIds) {  // visit all the neighbors and add them to the queue

            auto &way = waysMap.contains(wayId) ? waysMap.at(wayId):navigationGIS.getWay(wayId);
            waysMap.insert(std::pair<EntityId, const AbstractWay&>(wayId, way));
            if (Utils::isWayRestricted(way, restrictions)) {
                continue;
            }
            std::shared_ptr<Node> neighbor = createNeighbor(currNode, way, heuristicFunc, costFunc);
            EntityId neighborJunctionId = neighbor->getJunctionId();
            if (minNodes.find(neighborJunctionId) == minNodes.end() ||
                neighbor->getPriority() <= minNodes.find(neighborJunctionId)->second) {
                // add to queue only if the priority is <= from the current min priority of the junction
                if (!(minNodes.find(neighborJunctionId) == minNodes.end())) {
                    minNodes.erase(neighborJunctionId);
                }
                minNodes.insert(std::pair(neighborJunctionId, neighbor->getPriority()));
                queue.push(neighbor);
            }
        }
    }
/*-------------------------------- After search finished --------------------------------*/
    if (currNode->getJunctionId() != EntityId("")) {
        // no Route was found, return invalid route. Empty entityId means currNode is a final Node represents destination point
        return nullptr;
    }

    std::vector<std::pair<EntityId, Direction>> ways = restoreShortestRoute(currNode);
    return std::make_unique<Route>(origin, destination, currNode->getDistanceSoFar(), currNode->getTimeSoFar(), ways);
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
    return (double) Utils::calculateTime(CoordinatesMath::calculateDistance(coordinates, destination), MAX_SPEED);
}

double AStar::costByDistance(const AbstractWay &way) {
    return (double) way.getLength();
}

double AStar::costByTime(const AbstractWay &way) {
    return (double) Utils::getWayDuration(way.getLength(), way.getSpeedLimit());
}

bool AStar::compareByDistance(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
    if (node1->getPriority() == node2->getPriority()) {
        if (node1->getTimeSoFar() == node2->getTimeSoFar()) {
            return node1->getWaysCount() > node2->getWaysCount();
        }
        return node1->getTimeSoFar() > node2->getTimeSoFar();
    }
    return node1->getPriority() > node2->getPriority();
}

bool AStar::compareByTime(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
    if (node1->getPriority() == node2->getPriority()) {
        if (node1->getDistanceSoFar() == node2->getDistanceSoFar()) {
            return node1->getWaysCount() > node2->getWaysCount();
        }
        return node1->getDistanceSoFar() > node2->getDistanceSoFar();
    }
    return node1->getPriority() > node2->getPriority();
}

std::shared_ptr<AStar::Node> AStar::createNeighbor(std::shared_ptr<Node> currNode, const AbstractWay &way,
                                                   double (*heuristicFunc)(const Coordinates &, const Coordinates &),
                                                   double (*costFunc)(const AbstractWay &)) {
    auto idPair = way.getJunctions();
    auto fromId = idPair.first, toId = idPair.second;
    Direction direction =
            fromId == currNode->getJunctionId() ? Direction::A_to_B : Direction::B_to_A;
    EntityId neighborId = direction == Direction::A_to_B ? toId : fromId;
    Coordinates neighborCoordinates = direction == Direction::A_to_B ? way.getToJunctionCoordinates()
                                                                     : way.getFromJunctionCoordinates();
    Meters distanceSoFar = currNode->getDistanceSoFar() + way.getLength();
    Minutes timeSoFar = currNode->getTimeSoFar() + Utils::getWayDuration(way.getLength(), way.getSpeedLimit());
    double costSoFar = currNode->getCostSoFar() + costFunc(way);
    double priority = costSoFar + heuristicFunc(neighborCoordinates, destination);

    std::shared_ptr<Node> neighbor = std::make_shared<Node>(neighborCoordinates, neighborId, distanceSoFar,
                                                            timeSoFar, costSoFar, priority,
                                                            currNode->getWaysCount() + 1,
                                                            std::make_pair(way.getId(), direction), currNode);
    return neighbor;
}

std::shared_ptr<AStar::Node> AStar::createInitialNode(double (*heuristicFunc)(const Coordinates &, const Coordinates &),
                                                      double (*costFunc)(const AbstractWay &), Direction direction) {
    //  If direction is A_To_B then the Node start at "to" junction, else from "from" junction.
    //  We implemented the algorithm such that the initial Node already has a "kilometrage" of the startWay
    Coordinates initialCoordinates = direction == Direction::A_to_B ? startWay.getToJunctionCoordinates()
                                                                    : startWay.getFromJunctionCoordinates();

    auto distanceFromEdges = startWay.getSegmentPartsOnWay(startWaySegment, origin);
    //  the distance from origin point to the initial node
    Meters initialDistance = direction == Direction::A_to_B ? distanceFromEdges.second : distanceFromEdges.first;
    //  the time from origin point to the initial node
    Minutes initialTime = Utils::calculateTime(initialDistance, startWay.getSpeedLimit());
    auto idPair = startWay.getJunctions();
    auto fromId = idPair.first;
    auto toId = idPair.second;
    EntityId initialJunctionId =
            direction == Direction::A_to_B ? toId : fromId;
//    TODO find a better solution to update the initial cost
    double initialCost =
            costFunc == costByTime ? static_cast<double>(initialTime) : static_cast<double>(initialDistance);
    double initialPriority = heuristicFunc(initialCoordinates, destination) + initialCost;
    std::shared_ptr<Node> initialNode = std::make_shared<Node>(initialCoordinates, initialJunctionId,
                                                               initialDistance, initialTime, initialCost,
                                                               initialPriority, 1,
                                                               Edge(std::make_pair(startWay.getId(),
                                                                                   direction)),
                                                               nullptr);
    return initialNode;
}

std::shared_ptr<AStar::Node> AStar::createFinalNode(std::shared_ptr<Node> currNode,
                                                    double (*costFunc)(const AbstractWay &)) {
    auto idPair = finalWay.getJunctions();
    auto fromId = idPair.first;
    auto tiId = idPair.second;
    Direction direction =
            fromId == currNode->getJunctionId() ? Direction::A_to_B : Direction::B_to_A;

    // currNode represents the opposite junction of endCoordinates
    auto distanceFromEdges = finalWay.getSegmentPartsOnWay(finalWaySegment, destination);

    Meters distanceFromCurrNode = direction == Direction::A_to_B ? distanceFromEdges.first : distanceFromEdges.second;
    //  the total time to reach destination point
    Minutes finalTime = Utils::calculateTime(distanceFromCurrNode, finalWay.getSpeedLimit()) + currNode->getTimeSoFar();
    //  the total distance to reach destination point
    Meters finalDistance = distanceFromCurrNode + currNode->getDistanceSoFar();
    double finalCost = costFunc == costByTime ? (double) finalTime : (double) finalDistance;
    double finalPriority = finalCost; // reached destination so priority = cost :)
    std::shared_ptr<Node> finalNode = std::make_shared<Node>(destination, EntityId(""),
                                                             finalDistance, finalTime, finalCost,
                                                             finalPriority, currNode->getWaysCount() + 1,
                                                             Edge(std::make_pair(finalWay.getId(),
                                                                                 direction)),
                                                             currNode);
    return finalNode;
}

std::optional<std::unique_ptr<Route>> AStar::getSingleWayRoute() {
    std::optional<std::unique_ptr<Route>> route;
    if (!startWay.isBidirectional()) {
        Meters originsDistanceFromStart = startWay.getSegmentPartsOnWay(startWaySegment, origin).first;
        Meters destinationsDistanceFromStart = startWay.getSegmentPartsOnWay(finalWaySegment, destination).first;
        if (destinationsDistanceFromStart > originsDistanceFromStart) {
            Meters length = destinationsDistanceFromStart - originsDistanceFromStart;
            Minutes duration = Utils::getWayDuration(length, startWay.getSpeedLimit());
            std::pair<EntityId, Direction> edge = std::make_pair(startWay.getId(), Direction::A_to_B);
            std::vector<std::pair<EntityId, Direction>> ways{edge};
            route.emplace(std::make_unique<Route>(origin, destination, length, duration,
                                                  std::vector<std::pair<EntityId, Direction>>{edge}));
        }
    } else {
        Meters originsDistanceFromStart = startWay.getSegmentPartsOnWay(startWaySegment, origin).first;
        Meters destinationsDistanceFromStart = startWay.getSegmentPartsOnWay(finalWaySegment, destination).first;
        Meters length(std::fabs(static_cast<double>(originsDistanceFromStart - destinationsDistanceFromStart)));
        Minutes duration = Utils::getWayDuration(length, startWay.getSpeedLimit());
        Direction direction =
                originsDistanceFromStart < destinationsDistanceFromStart ? Direction::A_to_B : Direction::B_to_A;
        std::pair<EntityId, Direction> edge = std::make_pair(startWay.getId(), direction);
        route.emplace(std::make_unique<Route>(origin, destination, length, duration,
                                              std::vector<std::pair<EntityId, Direction>>{edge}));
    }
    return route;
}


AStar::Node::Node(const Coordinates &coordinates, const EntityId &junctionId,
                  const Meters &distanceSoFar, const Minutes &timeSoFar, double costSoFar, double priority,
                  unsigned int waysCount,
                  const Edge &prevEdgeWay, std::shared_ptr<Node> prevNode) : coordinates(coordinates),
                                                                             junctionId(junctionId),
                                                                             distanceSoFar(distanceSoFar),
                                                                             timeSoFar(timeSoFar),
                                                                             costSoFar(costSoFar),
                                                                             priority(priority),
                                                                             waysCount(waysCount),
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

unsigned int AStar::Node::getWaysCount() const {
    return waysCount;
}

