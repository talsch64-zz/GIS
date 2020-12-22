#ifndef GIS_CPP_RANDTESTUTILS_H
#define GIS_CPP_RANDTESTUTILS_H

#include "../GIS.h"
#include "Bound.h"
#include "../entities/Entity.h"
#include "../entities/Junction.h"
#include "../entities/POI.h"
#include "../entities/Way.h"
#include "GISMock.h"
#include "../navigation/Routes.h"
#include "RouteMock.h"
#include "../NavigationGIS.h"

class RandTestUtils {
public:
    static int randInt(int min, int max);

    static double randDouble(double min, double max);

    static bool randBool();

    static Coordinates randCoord(Bound bound);

    static Coordinates randCoord();

    static std::vector<Junction *>
    generateJunctions(std::unique_ptr<GISMock> gis, IdGenerator *idGenerator, int n, Bound bound);

    static std::vector<Way *>
    generateWays(std::unique_ptr<GISMock> gis, IdGenerator *idGenerator, int n, Bound bound,
                 std::vector<Junction *> junctions);

    static Routes
    getBestRoutes(GISMock &gis, IdGenerator &idGenerator, RouteMock &bestTimeRoute, RouteMock &bestDistanceRoute,
                  const Coordinates &start, const Coordinates &end);

    static void getBestRoutesDFS(NavigationGIS &navGis, RouteMock &bestTimeRoute, RouteMock &bestDistanceRoute,
                                 std::vector<std::pair<EntityId, Direction>> ways, const EntityId &start,
                                 const EntityId &end,
                                 const EntityId &current, Meters currentLength, Minutes currentTime);
};


#endif //GIS_CPP_RANDTESTUTILS_H
