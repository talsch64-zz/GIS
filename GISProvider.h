#pragma once

#include "GIS/GIS_315524694.h"

/// GISProvider provides a GIS instance.
class GISProvider {
	GISProvider() {}	

public: 
	static GIS_315524694 getGIS() {
		return GIS_315524694{};
	}
};