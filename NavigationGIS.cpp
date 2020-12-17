#include "NavigationGIS.h"
#include "CoordinatesMath.h"
#include "entities/Way.h"

NavigationGIS::NavigationGIS(const GIS &gis) : gis(gis) {
}

std::vector<EntityId> NavigationGIS::getWaysByJunction(const EntityId &junctionId) const {
    return gis.getWaysByJunction(junctionId);
}

std::pair<Coordinates, EntityId> NavigationGIS::getWayClosestPoint(const Coordinates &coords) const {
    auto closestWayPair = gis.getWayClosestPoint(coords);
    const Way &closestWay = getWay(closestWayPair.second);
    bool valid = true;
    if (closestWay.isHighway()) {
        Coordinates closestCoord = closestWay.getGeometry()->getClosestPoint(coords);
        Meters distance = CoordinatesMath::calculateDistance(closestCoord, coords);
        //TODO: is Meters comparison correct?
        if (distance > max_distance_from_highway) {
            valid = false;
        }
    }

    if (!valid) {
        closestWayPair = gis.getWayClosestPoint(coords, Restrictions("highway"));
    }
    return closestWayPair;
}

std::pair<Coordinates, EntityId>
NavigationGIS::getWayClosestPoint(const Coordinates &coords, const Restrictions &res) const {
    return gis.getWayClosestPoint(coords, res);
}

const Way &NavigationGIS::getWay(const EntityId &wayId) const {
    return gis.getWay(wayId);
}
