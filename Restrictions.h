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

    const std::unordered_set<std::string> &getRestrictions() const;

private:
//    TODO move to Utils.h or something
    std::unordered_set<std::string> split(const char* restrictions, const char *delimiter);
};

#endif //GIS_CPP_RESTRICTIONS_H
