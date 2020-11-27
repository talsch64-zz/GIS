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

    static Coordinates calculateCoordinatesByDistanceAndBearing(const Coordinates &c, Meters distance, double bearing) {
        double angDistance = distance / earth_radius;
        double rad_bearing = deg2rad(bearing);
        Latitude latitude(deg2rad(c.latitude()));
        Longitude longitude(deg2rad(c.longitude()));
        double targetLat = asin(sin(latitude) * cos(angDistance) + cos(latitude) * sin(angDistance) * cos(rad_bearing));
        double targetLon = longitude + atan2(sin(rad_bearing) * sin(angDistance) * cos(latitude),
                                             cos(angDistance) - sin(latitude) * sin(targetLat));
        Longitude targetLonDeg(rad2deg(targetLon));
        Latitude targetLatDeg(rad2deg(targetLat));
        return Coordinates(targetLonDeg, targetLatDeg);
    }

    static double calculateBearing(const Coordinates &start, const Coordinates &end) {
        // convert to radians
        double lat1 = deg2rad(start.latitude());
        double lng1 = deg2rad(start.longitude());
        double lat2 = deg2rad(end.latitude());
        double lng2 = deg2rad(end.longitude());
        double y = sin(lng2 - lng1) * cos(lat2);
        double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lng2 - lng1);
        double bearing = atan2(y, x);
        return rad2deg(bearing);
    }

    static Coordinates calculateMidpoint(const Coordinates &coord1, const Coordinates &coord2) {
        // convert to radians
        double lat1 = deg2rad(coord1.latitude());
        double lng1 = deg2rad(coord1.longitude());
        double lat2 = deg2rad(coord2.latitude());
        double lng2 = deg2rad(coord2.longitude());

        double Bx = cos(lat2) * cos(lng2 - lng1);
        double By = cos(lat2) * sin(lng2 - lng1);

        double latMidRad = atan2(sin(lat1) + sin(lat2), std::sqrt((cos(lat1) + Bx) * (cos(lat1) + Bx) + By * By));
        double lonMidRad = lng1 + atan2(By, cos(lat1) + Bx);

//        convert back to degrees
        return Coordinates(Longitude(rad2deg(lonMidRad)), Latitude(rad2deg(latMidRad)));

    }
/* calculate the closest point on AB from C and the shortest distance */
    static std::pair<Meters, Coordinates> calculateClosestPointAndDistanceAlongLine(const Coordinates &A, const Coordinates &B, const Coordinates &C) {
        double bearAB = deg2rad(calculateBearing(A, B));
        double bearAC = deg2rad(calculateBearing(A, C));
        double dAC = calculateDistance(A, C);
        if (fabs(bearAC - bearAB) > std::numbers::pi / 2) {
            return std::pair<Meters, Coordinates> {dAC, A};
        }
        double dxt = asin(sin(dAC / earth_radius) * sin(bearAC - bearAB)) * earth_radius;
        double dAB = calculateDistance(A, B);
        double dAD = acos(cos(dAC / earth_radius) / cos(dxt / earth_radius)) * earth_radius;
        if (dAD > dAB) {
            return std::pair<Meters, Coordinates> {calculateDistance(B,C), B};
        }
        double dat = acos(cos(dAC / earth_radius) / cos(dxt / earth_radius)) * earth_radius;
        Coordinates closest = calculateCoordinatesByDistanceAndBearing(A, Meters(dat), rad2deg(bearAB));
        return std::pair<Meters, Coordinates> {std::abs(dxt), closest};
    }

    /* calculate the shortest distance from C to AB*/
    static Meters calculateShortestDistanceFromLine(Coordinates &A, Coordinates &B, Coordinates &C) {
        double bearAB = deg2rad(calculateBearing(A, B));
        double bearAC = deg2rad(calculateBearing(A, C));
        double dAC = calculateDistance(A, C);
        if (fabs(bearAC - bearAB) > std::numbers::pi / 2) {
            return Meters(dAC);
        }
        double dxt = asin(sin(dAC / earth_radius) * sin(bearAC - bearAB)) * earth_radius;
        double dAB = calculateDistance(A, B);
        double dAD = acos(cos(dAC / earth_radius) / cos(dxt / earth_radius)) * earth_radius;
        if (dAD > dAB) {
            return calculateDistance(B, C);
        }
        return Meters(std::abs(dxt));
    }

    /* calculate the closest point on AB from C */
    static Coordinates calculateClosestCoordinatesAlongLine(Coordinates &A, Coordinates &B, Coordinates &C) {
        double bearAB = deg2rad(calculateBearing(A, B));
        double bearAC = deg2rad(calculateBearing(A, C));
        double dAC = calculateDistance(A, C);
        if (fabs(bearAC - bearAB) > std::numbers::pi / 2) {
            return A;
        }
        double dxt = asin(sin(dAC / earth_radius) * sin(bearAC - bearAB)) * earth_radius;
        double dAB = calculateDistance(A, B);
        double dAD = acos(cos(dAC / earth_radius) / cos(dxt / earth_radius)) * earth_radius;
        if (dAD > dAB) {
            return B;
        }
        double dat = acos(cos(dAC / earth_radius) / cos(dxt / earth_radius)) * earth_radius;
        Coordinates closest = calculateCoordinatesByDistanceAndBearing(A, Meters(dat), rad2deg(bearAB));
        return closest;
    }
};
