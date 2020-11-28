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

    static Coordinates CoordinatesByBearingAndDistance(const Coordinates &c, Meters distance, double bearing) {
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
/* calculates the closest point on A1A2 from C and the shortest distance */
    static std::pair<Meters, Coordinates> closestPointOnSegmentAndDistance(const Coordinates &A1, const Coordinates &A2, const Coordinates &C) {
        Coordinates closest = closestCoordinatesOnSegment(A1, A2, C);
        return std::pair<Meters, Coordinates> {calculateDistance(C, closest), closest};
    }

    /* calculates the shortest distance from C to A1A2*/
    static Meters distanceFromSegment(const Coordinates &A1, const Coordinates &A2, const Coordinates &C) {
        double bearing_A1A2 = deg2rad(calculateBearing(A1, A2));
        double bearing_A1C = deg2rad(calculateBearing(A1, C));
        double dAC = calculateDistance(A1, C);
        if (fabs(bearing_A1C - bearing_A1A2) > std::numbers::pi / 2) {
            return Meters(dAC);
        }
        double dxt = asin(sin(dAC / earth_radius) * sin(bearing_A1C - bearing_A1A2)) * earth_radius;
        double distance_A1A2 = calculateDistance(A1, A2);
        double dat = acos(cos(dAC / earth_radius) / cos(dxt / earth_radius)) * earth_radius;
        if (dat > distance_A1A2) {
            return calculateDistance(A2, C);
        }
        return Meters(std::abs(dxt));
    }

    /* calculates the closest point on A1A2 from C */
    static Coordinates closestCoordinatesOnSegment(const Coordinates &A1, const Coordinates &A2, const Coordinates &C) {
        double bearing_A1A2 = deg2rad(calculateBearing(A1, A2));
        double bearing_A1C = deg2rad(calculateBearing(A1, C));
        double distance_A1C = calculateDistance(A1, C);
        if (fabs(bearing_A1C - bearing_A1A2) > std::numbers::pi / 2) {
            return A1;
        }
        double dxt = asin(sin(distance_A1C / earth_radius) * sin(bearing_A1C - bearing_A1A2)) * earth_radius;
        double distance_A1A2 = calculateDistance(A1, A2);
        double dat = acos(cos(distance_A1C / earth_radius) / cos(dxt / earth_radius)) * earth_radius;
        if (dat > distance_A1A2) {
            return A2;
        }
        Coordinates closest = CoordinatesByBearingAndDistance(A1, Meters(dat), rad2deg(bearing_A1A2));
        return closest;
    }

    static Coordinates closestPointOnCircle(const Coordinates& c, const Coordinates& circle_center, Meters circle_radius) {
        Meters distance = CoordinatesMath::calculateDistance(circle_center, c);
        if (distance <= circle_radius) {
            return c;
        }
        double bearing = CoordinatesMath::calculateBearing(circle_center, c);
        return CoordinatesMath::CoordinatesByBearingAndDistance(circle_center, circle_radius, bearing);
    }

};


//    static Coordinates intersection(const Coordinates& cA1, const Coordinates& cA2, const Coordinates& cB1, const Coordinates& cB2) {
//        double bearingA = deg2rad(calculateBearing(cA1, cA2));
//        double bearingB = deg2rad(calculateBearing(cB1, cB2));
//        double lat_A1 = deg2rad(cA1.latitude());
//        double lat_B1 = deg2rad(cB1.latitude());
//        double lng_A1 = deg2rad(cA1.longitude());
//        double lng_B1 = deg2rad(cB1.longitude());
//
//        double angDistance_A1B1 = 2*asin(sqrt(pow(sin(abs(lat_A1 - lat_B1) / 2), 2) +
//                cos(lat_A1) * cos(lat_B1) * pow(sin(abs(lng_A1 - lng_B1)/2), 2)));
//
//        double bearing_a = acos((sin(lat_B1) - sin(lat_A1) * cos(angDistance_A1B1)) / (sin(angDistance_A1B1) * cos(lat_A1)));
//        double bearing_b = acos((sin(lat_A1) - sin(lat_B1) * cos(angDistance_A1B1)) / (sin(angDistance_A1B1) * cos(lat_B1)));
//
//        double bearing_A1B1;
//        double bearing_B1A1;
//
//        if (sin(lng_B1 - lng_A1) > 0) {
//            bearing_A1B1 = bearing_a;
//            bearing_B1A1 = 2*std::numbers::pi - bearing_b;
//        }
//        else {
//            bearing_A1B1 = 2*std::numbers::pi - bearing_a;
//            bearing_B1A1 = bearing_b;
//        }
//
//        double bearing_A1A2 = deg2rad(calculateBearing(cA1, cA2));
//        double bearing_B1B2 = deg2rad(calculateBearing(cB1, cB2));
//
//        double angle1 = bearing_A1A2 - bearing_A1B1;
//        double angle2 = bearing_B1A1 - bearing_B1B2;
//        double angle3 = acos(-cos(angle1) * cos(angle2) + sin(angle1) * sin(angle2) * cos(angDistance_A1B1));
//        double angDistance_AX = atan2(sin(angDistance_A1B1) * sin(angle1) * sin(angle2), cos(angle2) + cos(angle1) * cos(angle3));
//        double lat_X = asin(sin(lat_A1) * cos(angDistance_AX) + cos(lat_A1) * sin(angDistance_AX) * cos(bearing_A1A2));
//        double deltaLng = atan2(sin(bearing_A1A2) * sin(angDistance_AX) * cos(lat_A1), cos(angDistance_AX) - sin(lat_A1) * sin(lat_X));
//        double lng_X = lng_A1 + deltaLng;
//
//        return Coordinates(Longitude(rad2deg(lng_X)), Latitude(rad2deg(lat_X)));
//    }