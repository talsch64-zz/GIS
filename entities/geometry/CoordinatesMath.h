#pragma once

#include <cmath>

namespace std::numbers {
    static double pi = M_PI;
}
//TODO: revert pi back to its original form (nova recognizes numbers.h)

#include "../../GISdefs.h"

class CoordinatesMath {

    static constexpr double earth_radius = 6371000;
    // The function convert degrees into radians
    static double deg2rad(double deg) { return deg * std::numbers::pi / 180; }

    static double rad2deg(double rad) { return 180 * rad / std::numbers::pi; }

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
                         std::cos(latitude1) * std::cos(latitude2) * std::sin(delta_longitude / 2) *
                         std::sin(delta_longitude / 2);
        const double distance = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a)) * earth_radius;
        return Meters{distance};
    }

    static Coordinates calculateCoordinateByDistance(const Coordinates &c, Meters distance, double bearing) {
        double angDistance = distance / earth_radius;
        double rad_bearing = deg2rad(bearing);
        Latitude latitude(deg2rad(c.latitude()));
        Longitude longitude(deg2rad(c.longitude()));
        double targetLat = asin(sin(latitude)*cos(angDistance)+cos(latitude)*sin(angDistance)*cos(rad_bearing));
        double targetLon = longitude + atan2(sin(rad_bearing)*sin(angDistance)*cos(latitude), cos(angDistance)-sin(latitude)*sin(targetLat));
        Longitude targetLonDeg(rad2deg(targetLon));
        Latitude targetLatDeg(rad2deg(targetLat));
        return Coordinates(targetLonDeg, targetLatDeg);
    }

    static double calculateBearing(Coordinates &start, Coordinates &end) {
        // convert to radians
        double lat1 = deg2rad(start.latitude());
        double lon1 = deg2rad(start.longitude());
        double lat2 = deg2rad(end.latitude());
        double lon2 = deg2rad(end.longitude());
        double y = sin(lon2-lon1) * cos(lat2);
        double x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(lon2-lon1);
        double bearing = atan2(y,x);
        return rad2deg(bearing);
    }

    static Coordinates calculateMidpoint(const Coordinates& coord1, const Coordinates& coord2) {
        // convert to radians
        double lat1 = deg2rad(coord1.latitude());
        double lon1 = deg2rad(coord1.longitude());
        double lat2 = deg2rad(coord2.latitude());
        double lon2 = deg2rad(coord2.longitude());

        double Bx = cos(lat2) * cos(lon2 - lon1);
        double By = cos(lat2) * sin(lon2 - lon1);

        double latMidRad = atan2(sin(lat1) + sin(lat2), std::sqrt((cos(lat1)+Bx)*(cos(lat1)+Bx) + By*By));
        double lonMidRad = lon1 + atan2(By, cos(lat1) + Bx);

//        convert back to degrees
        return Coordinates(Longitude(rad2deg(lonMidRad)), Latitude(rad2deg(latMidRad)));

    }
};