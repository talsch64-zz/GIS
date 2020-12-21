#ifndef GIS_CPP_RANDTESTUTILS_H
#define GIS_CPP_RANDTESTUTILS_H

#include "../GIS.h"
#include "Bound.h"
#include "../entities/Entity.h"
#include "../entities/Junction.h"
#include "../entities/POI.h"
#include "../entities/Way.h"
#include "GISMock.h"

class RandTestUtils {
    static constexpr int CURVES_AMOUNT = 2;

    static int randInt(int min, int max);

    static double randDouble(double min, double max);

    static bool randBool();

    static Coordinates randCoord(Bound bound);

    static Coordinates randCoord();

    static std::vector<Junction *>
    generateJunctions(std::unique_ptr<GISMock> gis, IdGenerator *idGenerator, int n, Bound bound);

    static std::vector<Way *>
    generateWays(std::unique_ptr<GISMock> gis, IdGenerator *idGenerator, int n, std::vector<Junction *> junctions);
};


#endif //GIS_CPP_RANDTESTUTILS_H
