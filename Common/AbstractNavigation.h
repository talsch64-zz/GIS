#pragma once

#include "GISNamedTypes.h"
#include "AbstractRoutes.h"
#include "AbstractNavigation.h"
#include <memory> 

class AbstractNavigation {
public:
  virtual ~AbstractNavigation() {}
  virtual std::unique_ptr<AbstractRoutes> getRoutes(const Coordinates& start, const Coordinates& end) const = 0;
};

