#pragma once 

#include "GISNamedTypes.h"
#include "Restrictions.h"
#include "AbstractWay.h"
#include <vector>
#include <tuple>

class AbstractGIS {
public:
  virtual ~AbstractGIS() {}
  virtual std::vector<EntityId> loadMapFile(const std::string& filename) = 0;
  virtual std::vector<EntityId> getWaysByJunction(const EntityId& junctionId) const = 0;
  virtual std::tuple<Coordinates, EntityId, std::size_t> getWayClosestPoint(const Coordinates& coords) const = 0;
  virtual std::tuple<Coordinates, EntityId, std::size_t> getWayClosestPoint(const Coordinates& coords, const Restrictions& res) const = 0;
  virtual const AbstractWay& getWay(const EntityId& wayId) const = 0;
};

