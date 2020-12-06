#pragma once

#include "GIS.h"

class GISProvider {
	GISProvider() {}	

public: 
	static GIS getGIS() {
		return GIS{};
	}
};