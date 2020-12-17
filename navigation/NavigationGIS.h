

#ifndef GIS_CPP_NAVIGATIONGIS_H
#define GIS_CPP_NAVIGATIONGIS_H

#include "../GIS.h"
#include "../GISNamedTypes.h"
class NavigationGIS {
    const GIS& gis;
public:
    NavigationGIS(const GIS& gis): gis(gis) {}

    std::vector<EntityId> getWaysByJunction(const EntityId& junctionId) const {
        return gis.getWaysByJunction(junctionId);
    }

    std::pair<Coordinates, EntityId> getWayClosestPoint(const Coordinates& coords) const {
        return gis.getWayClosestPoint(coords);
    }
// TODO implement Restriction and the following function

//    std::pair<Coordinates, EntityId>
//    getWayClosestPoint(const Coordinates& coords, const Restrictions& res) const {
//        return gis.getWayClosestPoint(coords, res);
//    }

    const Way& getWay(const EntityId& wayId) const {
        return gis.getWay(wayId);
    }

};



#endif //GIS_CPP_NAVIGATIONGIS_H
