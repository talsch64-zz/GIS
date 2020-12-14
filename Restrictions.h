//
// Created by student on 14/12/2020.
//

#ifndef GIS_CPP_RESTRICTIONS_H
#define GIS_CPP_RESTRICTIONS_H


#include <unordered_set>
#include <string>

/**
 * @brief Restrictions class
 *
 */
class Restrictions {
    std::unordered_set<std::string> restrictions;
public:
    /**
     * @brief Construct a new Restrictions object
     *
     * @param restrictions
     */
    Restrictions(const char* restrictions);

private:
    std::unordered_set<std::string> splitRestrictions(const char* restrictions);
};

#endif //GIS_CPP_RESTRICTIONS_H
