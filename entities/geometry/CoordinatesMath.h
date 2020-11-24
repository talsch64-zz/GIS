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

    static double calculateBearing(const Coordinates &start, const Coordinates &end) {
        // convert to radians
        double lat1 = deg2rad(start.latitude());
        double lng1 = deg2rad(start.longitude());
        double lat2 = deg2rad(end.latitude());
        double lng2 = deg2rad(end.longitude());
        double y = sin(lng2 - lng1) * cos(lat2);
        double x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(lng2 - lng1);
        double bearing = atan2(y,x);
        return rad2deg(bearing);
    }

    static Coordinates calculateMidpoint(const Coordinates& coord1, const Coordinates& coord2) {
        // convert to radians
        double lat1 = deg2rad(coord1.latitude());
        double lng1 = deg2rad(coord1.longitude());
        double lat2 = deg2rad(coord2.latitude());
        double lng2 = deg2rad(coord2.longitude());

        double Bx = cos(lat2) * cos(lng2 - lng1);
        double By = cos(lat2) * sin(lng2 - lng1);

        double latMidRad = atan2(sin(lat1) + sin(lat2), std::sqrt((cos(lat1)+Bx)*(cos(lat1)+Bx) + By*By));
        double lonMidRad = lng1 + atan2(By, cos(lat1) + Bx);

//        convert back to degrees
        return Coordinates(Longitude(rad2deg(lonMidRad)), Latitude(rad2deg(latMidRad)));

    }

    static Coordinates calculateClosestCoordinateAlongLine(Coordinates& A, Coordinates& B, Coordinates& C) {
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
        Coordinates closest = calculateCoordinateByDistance(A, Meters(dat), rad2deg(bearAB));
        return closest;
    }
};


//TODO delete all the remarks bellow

//    static Coordinates calculateClosestCoordinateAlongLine(const Coordinates &start, const Coordinates &end, const Coordinates &target) {
//        Coordinates A = start;
//        Coordinates B = end;
//        const Coordinates &C = target;
//
//        double AB = calculateDistance(A, B);
//        double AC = calculateDistance(A, C);
//        double BC = calculateDistance(B, C);
//
//        if(BC > AB && C.latitude() > B.latitude()) {
////            swap between A and B
//            Coordinates *tmpPtr = &B;
//            *(&B) = A;
//            *(&A) = *tmpPtr;
//            double tmpVal = AC;
//            AC = BC;
//            BC = tmpVal;
//        }
//
//        double bearAB = deg2rad(calculateBearing(A, B));
//        double bearAC = deg2rad(calculateBearing(A, C));
//
//        double angDisAC = AC / earth_radius;
////        cross-track distance
//        double dxt = asin(sin(angDisAC) * sin(bearAC - bearAB)) * earth_radius;
////        along-track distance
//        double dat = acos(cos(angDisAC) / cos(dxt / earth_radius)) * earth_radius;
//
//        Coordinates closest = calculateCoordinateByDistance(A, Meters(dat), rad2deg(bearAB));
//
//        if (calculateDistance(A, closest) > AB || calculateDistance(B, closest) > AB) {
//            if (AC > BC) {
//                return B;
//            }
//            return A;
//        }
//        return closest;
//    }
//


///*calculate the coordinate along the great circle that runs through coordinates A and B which is closest to C*/
//    static Coordinates calculateClosestCoordinateAlongGreatGreatCircle(const Coordinates &A, const Coordinates &B, const Coordinates &C) {
//        double latA = A.latitude();
//        double latB= B.latitude();
//        double latC = C.latitude();
//        double lngA = A.longitude();
//        double lngB= B.longitude();
//        double lngC = C.longitude();
//        double t = ((latC - latA) * (latB - latA) + (lngC - lngA) * (lngB - lngA)) / ((latB - latA) * (latB - latA) + (lngB - lngA) *  (lngB - lngA));
//        return Coordinates(Longitude(lngA + t*(lngB-lngA)), Latitude(latA + t*(latB - latA)));
//    }
//
//    static Coordinates calculateClosestCoordinateAlongLine(const Coordinates &start, const Coordinates &end, const Coordinates &source) {
//        Coordinates closest = calculateClosestCoordinateAlongGreatGreatCircle(start, end, source);
////        if closest point is not on the line itselt
//        if ((closest.longitude() > start.longitude() && closest.longitude() > end.longitude()) ||
//                (closest.longitude() < start.longitude() && closest.longitude() < end.longitude()) ||
//                (closest.latitude() > start.latitude() && closest.latitude() > end.latitude()) ||
//                (closest.latitude() < start.latitude() && closest.latitude() < end.latitude())) {
//            if (calculateDistance(source, start) > calculateDistance(source, end)) {
//                closest = end;
//            }
//            else {
//                closest = start;
//            }
//        }
//        return closest;
//    }