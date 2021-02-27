#pragma once

#include "GIS.h"

class GISProvider {
public:
        static auto getGIS() {
                return GIS{};
        }
};
