//
// Created by student on 19/12/2020.
//

#ifndef GIS_CPP_UTILS_H
#define GIS_CPP_UTILS_H

#include "GISNamedTypes.h"
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


/**
 * @brief split string to tokens by given delimiters
 * @param array
 * @param delimiters
 * @return vector with all the tokens
 */
static std::vector<std::string> split(const char* array, const char *delimiters) {
        std::vector<std::string> vector;
        char arrayCopy[strlen(array)];
        strcpy(arrayCopy, array);
        char* token = strtok(arrayCopy, delimiters);
        while (token != nullptr) {
            vector.push_back(std::string(token));
            token = strtok (nullptr, delimiters);
        }
        return vector;
}

#endif //GIS_CPP_UTILS_H
