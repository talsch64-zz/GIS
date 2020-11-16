
#include "POI.h"

#include <utility>

POI::POI(const std::string &id, const std::string &name, const std::string &description,
         const std::vector<std::string> &categoryTags, std::vector<std::string> accessibility,
         std::unique_ptr<Geometry> geometry) : Entity(id, name, description, categoryTags),
                                                      accessibility(std::move(accessibility)), geometry(std::move(geometry)) {}
const std::vector<std::string> &POI::getAccessibility() const {
    return accessibility;
}
