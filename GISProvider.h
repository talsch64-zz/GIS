#pragma once

#include "GIS/GIS.h"

/// GISProvider provides a GIS instance.
class GISProvider {
	GISProvider() {}	

public: 
	static GIS getGIS() {
		return GIS{};
	}
};