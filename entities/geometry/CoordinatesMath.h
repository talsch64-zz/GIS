#pragma once

#include <cmath>

#include "../../GISdefs.h"

class CoordinatesMath {
    static constexpr double pi = 3.141592653589793238;
    // Earth radius in meters
    static constexpr double earth_radius = 6371000;

    // The function convert degrees into radians
    //TODO: fix pi
    static double deg2rad(double deg) { return deg * pi / 180; }

    static double rad2deg(double rad) { return 180 * rad / pi; }

public:
    // The function returns the airial distance in meters between two decimal coordinates
    //
    // From: https://www.movable-type.co.uk/scripts/latlong.html
    // Distance
    // This uses the "Haversine" formula to calculate the great-circle distance between two points that is, the shortest distance over the earth's surface.
    // Giving an "as-the-crow-flies" distance between the points (ignoring any hills they fly over, of course!).
    // formula:     a = sin²(DeltaLatitude/2) + cos(Latitude1) * cos(Latitude2) * sin²(DeltaLongtitude/2)
    // c = 2 * atan2(sqrt(a), sqrt(1-a))
    // d = R * c
    // where R is earth's radius (mean radius = 6,371km);
    // note that angles need to be in radians to pass to trig functions!
    //
    // See: https://en.wikipedia.org/wiki/Atan2
    static Meters calculateDistance(const Coordinates &c1, const Coordinates &c2) {
        const double latitude1 = deg2rad(c1.latitude());
        const double latitude2 = deg2rad(c2.latitude());
        const double longitude1 = deg2rad(c1.longitude());
        const double longitude2 = deg2rad(c2.longitude());
        const double delta_latitude = latitude2 - latitude1;
        const double delta_longitude = longitude2 - longitude1;
        const double a = std::sin(delta_latitude / 2) * std::sin(delta_latitude / 2) +
                         std::cos(latitude1) * std::cos(latitude2) * std::sin(delta_longitude) / 2 *
                         std::sin(delta_longitude / 2);
        const double distance = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a)) * earth_radius;
        return Meters{distance};
    }

    static Coordinates calculateCoordinateByDistance(const Coordinates &c, Meters distance, double deg) {
        double angDistance = distance / earth_radius;
        double radDeg = deg2rad(deg);
        Latitude latRad(deg2rad(c.latitude()));
        Longitude lonRad(deg2rad(c.longitude()));
        Latitude targetLat(std::asin(
                std::sin(latRad) * std::cos(angDistance) +
                std::cos(lonRad) * std::sin(angDistance) * std::cos(radDeg)));
        Longitude targetLon(lonRad + std::atan2(std::sin(radDeg) * std::sin(angDistance) * std::cos(latRad),
                                                std::cos(angDistance) - std::sin(latRad) * std::sin(targetLat)));

        Longitude targetLonDeg(rad2deg(targetLon));
        Latitude targetLatDeg(rad2deg(targetLat));
        return Coordinates(targetLonDeg, targetLatDeg);
    }
};