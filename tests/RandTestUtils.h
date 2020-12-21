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

class RandTestUtils {
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

    static void getBestRoutesDFS(RouteMock *bestTimeRoute, RouteMock *bestDistanceRoute, std::unordered_map<EntityId, Junction *> junctions,
                                 std::vector<std::pair<EntityId, Direction>> ways, Junction *start, Junction *end,
                                 Junction *current, Meters currentLength, Minutes currentTime);
};


#endif //GIS_CPP_RANDTESTUTILS_H
