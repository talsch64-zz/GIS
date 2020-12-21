#include "Bound.h"

Bound::Bound(const Latitude &minLat, const Latitude &maxLat, const Longitude &minLon, const Longitude &maxLon) : minLat(
        minLat), maxLat(maxLat), minLon(minLon), maxLon(maxLon) {}
