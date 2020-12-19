//
// Created by student on 19/12/2020.
//

#ifndef GIS_CPP_UTILS_H
#define GIS_CPP_UTILS_H

/**
 * @brief converd kilometers per hour to meters per minute
 * @param speed in kmh
 * @return speed in mm
 */
static inline double kmh_to_mm(int speed) {
    return (double) speed * 1000 / 60;
}

static inline Minutes calculateTime(Meters distance, int speed) {
    return Minutes((double) distance / kmh_to_mm(speed));
}

#endif //GIS_CPP_UTILS_H
