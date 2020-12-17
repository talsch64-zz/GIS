#pragma once

#include <vector>
#include <string>

/**
 * @brief Restrictions class
 * 
 */
class Restrictions {
    std::vector<std::string> restrictions;

public:
    Restrictions();

	/**
	 * @brief Construct a new Restrictions object
	 * 
	 * @param restrictions 
	 */
	Restrictions(const char* restrictions);

	std::vector<std::string> get() const;
};
