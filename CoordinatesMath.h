#pragma once

#include <numbers>
#include <cmath>
#include <tuple>
#include <optional>
#include "GISNamedTypes.h"

using std::sin;
using std::cos;
using std::pow;
using std::sqrt;
using std::atan2;
using std::fabs;
using std::fmod;
using std::fmin;
using std::fmax;

// A utility class that provides Sphere geometry calculations
//
// Note: You may add functionality to this class but do not change the implementation of the public API (once published) as this may result in a failures of the automatic test of your code.
//
class CoordinatesMath {
public:
    // Precision constants
    static constexpr double distance_epsilon = 1 / tenToThePowerOf<coordinate_precision + 3>();

    // Earth constants
    static constexpr double earth_radius = 6371000;
    static constexpr double n_pole_lat = 90;
    static constexpr double s_pole_lat = -90;

private:
    // The function convert degrees into radians
    static double deg2rad(double deg) { return deg * std::numbers::pi / 180; }

    // The function convert radians into degrees
    static double rad2deg(double rad) { return rad * 180 / std::numbers::pi; }

    // Constrain degrees to range 0..+360 (e.g. for bearing);
    static double wrap360(double deg) {
        if (0 < deg && deg <= 360) return deg; // avoid rounding due to arithmetic ops if within range
        return fmod(deg + 360, 360);
    }

    // Constrain degrees to range -180..+180 (e.g. for longitude); -181 => 179, 181 => -179.
    static double wrap180(double deg) {
        if (-180 < deg && deg <= 180) return deg; // avoid rounding due to arithmetic ops if within range
        return fmod(deg + 540, 360) - 180; // sawtooth wave p:180, a:±180
    }

    // Constrain degrees to range -90..+90 (e.g. for latitude); -91 => -89, 91 => 89.
    static double wrap90(double deg) {
        if (-90 <= deg && deg <= 90) return deg; // avoid rounding due to arithmetic ops if within range
        return fabs(fmod(fmod(deg, 360) + 270, 360) - 180) - 90; // triangle wave p:360 a:±90 TODO: fix e.g. -315°
    }

    static double rad2longitude(double rad) {
        return wrap180(rad2deg(rad));
    }

    static double rad2latitude(double rad) {
        return wrap90(rad2deg(rad));
    }

    static Coordinates rads2Coordinates(double rad_longitude, double rad_latitude) {
        return {Longitude{rad2longitude(rad_longitude)}, Latitude{rad2latitude(rad_latitude)}};
    }

    static auto cooridnates2radians(const Coordinates &c1, const Coordinates &c2) {
        return std::tuple{deg2rad(c1.longitude()), deg2rad(c1.latitude()), deg2rad(c2.longitude()),
                          deg2rad(c2.latitude())};
    }

    static auto cooridnates2radiansAndDeltaLongitude(const Coordinates &c1, const Coordinates &c2) {
        auto[longitude1, latitude1, longitude2, latitude2] = cooridnates2radians(c1, c2);
        return std::tuple{longitude1, latitude1, longitude2, latitude2, longitude2 - longitude1};
    }

    static auto cooridnates2radiansAndDeltas(const Coordinates &c1, const Coordinates &c2) {
        auto[longitude1, latitude1, longitude2, latitude2, delta_longitude] = cooridnates2radiansAndDeltaLongitude(c1,
                                                                                                                   c2);
        return std::tuple{longitude1, latitude1, longitude2, latitude2, delta_longitude, latitude2 - latitude1};
    }

    // The function returns the airial distance in meters between two decimal coordinates
    //
    // From: https://www.movable-type.co.uk/scripts/latlong.html
    // Distance
    // This uses the "Haversine" formula to calculate the great-circle distance between two points that is, the shortest distance over the earth's surface.
    // Giving an "as-the-crow-flies" distance between the points (ignoring any hills they fly over, of course!).
    // formula:     a = sin²(DeltaLatitude/2) + cos(Latitude1) * cos(Latitude2) * sin²(DeltaLongtitude/2)
    // d = 2 * atan2(sqrt(a), sqrt(1-a))
    // Where d is distance on 1-sphere
    // note that angles need to be in radians to pass to trig functions!
    static double calculateDistance1Sphere(const Coordinates &c1, const Coordinates &c2) {
        // convert to radians and compute delta_longitude
        const auto[longitude1, latitude1, longitude2, latitude2, delta_longitude, delta_latitude] = cooridnates2radiansAndDeltas(
                c1, c2);
        const double a =
                pow(sin(delta_latitude / 2), 2) + cos(latitude1) * cos(latitude2) * pow(sin(delta_longitude / 2), 2);
        const double distance = 2 * atan2(sqrt(a), sqrt(1 - a));
        return distance;
    }

    // The function returns the point of intersection of two paths defined by point and bearing
    static std::optional<Coordinates>
    intersection(const Coordinates &c1, double brng1, const Coordinates &c2, double brng2) {
        // see www.edwilliams.org/avform.htm#Intersection

        // convert to radians and compute delta_longitude
        const auto[longitude1, latitude1, longitude2, latitude2, delta_longitude, delta_latitude] = cooridnates2radiansAndDeltas(
                c1, c2);

        const double theta_13 = deg2rad(brng1);
        const double theta_23 = deg2rad(brng2);
        const double angular_distance_12 = 2 * asin(sqrt(sin(delta_latitude / 2) * sin(delta_latitude / 2) +
                                                         cos(latitude1) * cos(latitude2) * sin(delta_longitude / 2) *
                                                         sin(delta_longitude / 2)));

        if (angular_distance_12 < distance_epsilon) return {c1}; // coincident points

        // initial/final bearings between points
        const double cos_theta_a = (sin(latitude2) - sin(latitude1) * cos(angular_distance_12)) /
                                   (sin(angular_distance_12) * cos(latitude1));
        const double cos_theta_b = (sin(latitude1) - sin(latitude2) * cos(angular_distance_12)) /
                                   (sin(angular_distance_12) * cos(latitude2));
        const double theta_a = acos(fmin(fmax(cos_theta_a, -1), 1)); // protect against rounding errors
        const double theta_b = acos(fmin(fmax(cos_theta_b, -1), 1)); // protect against rounding errors

        const double theta_12 = sin(delta_longitude) > 0 ? theta_a : 2 * std::numbers::pi - theta_a;
        const double theta_21 = sin(delta_longitude) > 0 ? 2 * std::numbers::pi - theta_b : theta_b;

        const double alpha1 = theta_13 - theta_12; // angle 2-1-3
        const double alpha2 = theta_21 - theta_23; // angle 1-2-3

        if (sin(alpha1) == 0 && sin(alpha2) == 0) return {}; // infinite intersections
        if (sin(alpha1) * sin(alpha2) < 0) return {};        // ambiguous intersection (antipodal?)

        const double cos_alpha3 = -cos(alpha1) * cos(alpha2) + sin(alpha1) * sin(alpha2) * cos(angular_distance_12);

        const double angular_distance_13 = atan2(sin(angular_distance_12) * sin(alpha1) * sin(alpha2),
                                                 cos(alpha2) + cos(alpha1) * cos_alpha3);

        const double latitude3 = asin(fmin(fmax(
                sin(latitude1) * cos(angular_distance_13) + cos(latitude1) * sin(angular_distance_13) * cos(theta_13),
                -1), 1));

        const double delta_longitude_13 = atan2(sin(theta_13) * sin(angular_distance_13) * cos(latitude1),
                                                cos(angular_distance_13) - sin(latitude1) * sin(latitude3));
        const double longitude3 = longitude1 + delta_longitude_13;

        return {rads2Coordinates(longitude3, latitude3)};
    }

    static Meters
    returnDistance(const Coordinates &reference_point, const Coordinates &closest_point, Meters hint = Meters{-1}) {
        if (hint >= 0) return hint;
        return calculateDistance(reference_point, closest_point);
    }

    static auto returnClosestPointAndDistance(const Coordinates &reference_point, const Coordinates &closest_point,
                                              Meters hint = Meters{-1}) {
        return std::pair{closest_point, returnDistance(reference_point, closest_point, hint)};
    }

    static Coordinates
    returnClosestPoint(const Coordinates &reference_point, const Coordinates &closest_point, Meters hint = Meters{-1}) {
        (void) reference_point; //ignore parameter
        (void) hint; //ignore parameter
        return closest_point;
    }

    static double relativeBearing(double deg_brng1, double deg_brng2) {
        const double relative_bearing = fabs(deg_brng2 - deg_brng1);
        return (relative_bearing > 180 ? 360 - relative_bearing : relative_bearing);
    }

    static auto
    closestPointOnSegmentAndDistanceApplier(const Coordinates &c, const Coordinates &cA1, const Coordinates &cA2,
                                            auto set_return) {
        // see www.edwilliams.org/avform.htm#Intersection
        // Here, the great-circle path is identified by a start point cA1 and an end point, cA2
        if (c == cA1) return set_return(c, cA1, Meters{0});
        if (c == cA2) return set_return(c, cA2, Meters{0});
        const double bearing_cA1_cA2 = initialBearing(cA1, cA2);
        const double bearing_cA1_c = initialBearing(cA1, c);
        const double relative_bearing_from_cA1 = relativeBearing(bearing_cA1_cA2, bearing_cA1_c);
        if (relative_bearing_from_cA1 > 90) return set_return(c, cA1, Meters{-1}); // Is angle acute?
        double distance_1sphere_cA1_c = calculateDistance1Sphere(cA1, c);
        const double d_xt = asin(sin(distance_1sphere_cA1_c) * sin(deg2rad(bearing_cA1_c - bearing_cA1_cA2)));
        const double distance_1sphere_A = calculateDistance1Sphere(cA1, cA2);
        const double d_at = acos(cos(distance_1sphere_cA1_c) /
                                 cos(d_xt)); // The along-track distance from cA1 to closest point on path to the third point
        if (d_at >= distance_1sphere_A) return set_return(c, cA2, Meters{-1});
        const Coordinates closest_point = coordinatesByBearingAndDistance(cA1, bearing_cA1_cA2,
                                                                          Meters{d_at * earth_radius});
        return set_return(c, closest_point, Meters{earth_radius * fabs(d_xt)});
    }

public:
    // The function returns the airial distance in meters between two decimal coordinates on Earth
    //
    // This uses the "Haversine" formula to calculate the great-circle distance between two points that is, the shortest distance over the earth's surface.
    // Giving an "as-the-crow-flies" distance between the points (ignoring any hills they fly over, of course!).
    //
    static Meters calculateDistance(const Coordinates &c1, const Coordinates &c2) {
        return Meters{calculateDistance1Sphere(c1, c2) * earth_radius};
    }

    // The function returns the half-way point along a great circle path between the two points
    static Coordinates midpoint(const Coordinates &c1, const Coordinates &c2) {
        // convert to radians and compute delta_longitude
        const auto[longitude1, latitude1, longitude2, latitude2, delta_longitude] = cooridnates2radiansAndDeltaLongitude(
                c1, c2);
        const double b_x = cos(latitude2) * cos(delta_longitude);
        const double b_y = cos(latitude2) * sin(delta_longitude);
        const double midpoint_longitude = longitude1 + atan2(b_y, cos(latitude1) + b_x);
        const double midpoint_latitude = atan2(sin(latitude1) + sin(latitude2),
                                               sqrt(pow(cos(latitude1) + b_x, 2) + pow(b_y, 2)));
        return rads2Coordinates(midpoint_longitude, midpoint_latitude); // convert back to valid {Longitude, Latitude}
    }

    // The function returns the initial bearing between the two points:
    //
    // Returned bearing is normalised to: 0° - 360°
    static double initialBearing(const Coordinates &start, const Coordinates &end) {
        // convert to radians and compute delta_longitude
        const auto[start_longitude, start_latitude, end_longitude, end_latitude, delta_longitude] = cooridnates2radiansAndDeltaLongitude(
                start, end);
        const double y = sin(delta_longitude) * cos(end_latitude);
        const double x = cos(start_latitude) * sin(end_latitude) -
                         sin(start_latitude) * cos(end_latitude) * cos(delta_longitude);
        const double bearing = atan2(y, x);
        return wrap360(rad2deg(bearing)); // Normalised bearing in the range of 0° ... 360°
    }

    // The function returns the closest Coordinates to c along given segment A (provided by cA1 and cA2)
    static Coordinates closestPointOnSegment(const Coordinates &c, const Coordinates &cA1, const Coordinates &cA2) {
        return closestPointOnSegmentAndDistanceApplier(c, cA1, cA2, returnClosestPoint);
    }

    // The function returns the closest Coordinates to c along given segment A (provided by cA1 and cA2) and distance form A
    static std::pair<Coordinates, Meters>
    closestPointOnSegmentAndDistance(const Coordinates &c, const Coordinates &cA1, const Coordinates &cA2) {
        return closestPointOnSegmentAndDistanceApplier(c, cA1, cA2, returnClosestPointAndDistance);
    }

    // The function returns the distance of given Coordinates c from segment A (provided by cA1 and cA2)
    static Meters distanceFromSegment(const Coordinates &c, const Coordinates &cA1, const Coordinates &cA2) {
        return closestPointOnSegmentAndDistanceApplier(c, cA1, cA2, returnDistance);
    }

    // The function returns the closest Coordinates on a given circles' perimeter
    static Coordinates
    closestPointOnCircle(const Coordinates &c, const Coordinates &circle_center, Meters circle_radius) {
        const double distance = CoordinatesMath::calculateDistance(circle_center, c);
        if (distance <= circle_radius) return c;
        const double bearing = CoordinatesMath::initialBearing(circle_center, c);
        return CoordinatesMath::coordinatesByBearingAndDistance(circle_center, bearing, circle_radius);
    }

    // The function returns Coordinates of the intersection of given a Coordinate cA1 and its bearing toward cA2 and Coordinates cB1 and its bearing toward cB2
    //
    // Use this function with caution as it is less efficient
    static std::optional<Coordinates>
    intersection(const Coordinates &cA1, const Coordinates &cA2, const Coordinates &cB1, const Coordinates &cB2) {
        return intersection(cA1, initialBearing(cA1, cA2), cB1, initialBearing(cB1, cB2));
    }

    // The function returns Latitude of intersection with a given meridian
    //
    // Note: Use with caution, this function assumes input is valid (see intersection above)
    static Latitude intersectionOnMeridian(const Coordinates &cA1, const Coordinates &cA2, Longitude lng) {
        return intersection(cA1, cA2, Coordinates{lng, Latitude{n_pole_lat}},
                            Coordinates{lng, Latitude{s_pole_lat}})->latitude();
    }

    // The function returns terminal (destination) Coordinates given initial Coordinates, absolute_bearing (clockwise angle from north) in degrees and distance in meters
    //
    // initial - start Coordinates
    // absolute_bearing - 0° (North) to 360°
    // distance in Meters
    static Coordinates
    coordinatesByBearingAndDistance(const Coordinates &initial, double absolute_bearing, Meters distance) {
        const double rad_angular_distance = distance / earth_radius;
        const double rad_bearing = deg2rad(absolute_bearing);
        const double initial_latitude = deg2rad(initial.latitude());
        const double initial_longitude = deg2rad(initial.longitude());
        const double sin_destination_latitude = sin(initial_latitude) * cos(rad_angular_distance) +
                                                cos(initial_latitude) * sin(rad_angular_distance) * cos(rad_bearing);
        const double destination_latitude = std::asin(sin_destination_latitude);
        const double y = sin(rad_bearing) * sin(rad_angular_distance) * cos(destination_latitude);
        const double x = cos(rad_angular_distance) - sin(destination_latitude) * sin_destination_latitude;
        const double destination_longitude = initial_longitude + atan2(y, x);
        return rads2Coordinates(destination_longitude, destination_latitude);
    }
};

