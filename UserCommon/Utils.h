#ifndef GIS_CPP_UTILS_H
#define GIS_CPP_UTILS_H

#include <vector>
#include "../Common/GISNamedTypes.h"
#include "../Common/AbstractWay.h"
#include "../Common/Restrictions.h"

class Utils {
public:
    /**
     * Max distance to be from a highway for it to be a valid start of a route
     */
    static constexpr Meters max_distance_from_highway = Meters(3);

    /**
     * @brief converd kilometers per hour to meters per minute
     * @param speed in kmh
     * @return speed in mm
     */
    static double kmh_to_mm(int speed);

    static Minutes calculateTime(Meters distance, int speed);

    /**
     * @brief split string to tokens by given delimiters
     * @param array
     * @param delimiters
     * @return vector with all the tokens
     */
    static std::vector<std::string> split(const char *array, const char *delimiters);

    static bool isWayRestricted(const AbstractWay &way, const Restrictions &restrictions);

    /**
     * @brief converts speed kilometers per hour to meters per minute
     * @param speed
     * @return Minutes
     */
    static Minutes getWayDuration(Meters length, int speed);
};

#endif //GIS_CPP_UTILS_H
