#ifndef GIS_CPP_UTILS_H
#define GIS_CPP_UTILS_H

#include <vector>
#include "GISNamedTypes.h"

class Utils {
public:
    /**
     * @brief converd kilometers per hour to meters per minute
     * @param speed in kmh
     * @return speed in mm
     */
    static double kmh_to_mm(int speed);

    static inline Minutes calculateTime(Meters distance, int speed);

    /**
     * @brief split string to tokens by given delimiters
     * @param array
     * @param delimiters
     * @return vector with all the tokens
     */
    static std::vector<std::string> split(const char *array, const char *delimiters);
};

#endif //GIS_CPP_UTILS_H
