

#ifndef _PLANET_MOTION_H
#define _PLANET_MOTION_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// numeric constants...

#define METERS_PER_ASTRONOMICAL_UNIT        1.4959787e+11
#define METERS_PER_EARTH_EQUATORIAL_RADIUS  6378160.0
#define EARTH_RADII_PER_ASTRONOMICAL_UNIT   (METERS_PER_ASTRONOMICAL_UNIT/METERS_PER_EARTH_EQUATORIAL_RADIUS)

// macros

#define sqr(x)      sq(x)

#define RAD(x)      ((x)*(M_PI/180.0))
#define DEG(x)      ((x)*(180.0/M_PI))

// type definitions

typedef struct cartesian_coordinates_s {
    float x;
    float y;
    float z;
    float au;               // radius in AU
    float day;
} cartesian_coordinates_t;

typedef struct planet_s {
    char const * name;      // name of the object, e.g. "Mars".
    float N0, Nc;           // N0 = longitude of the ascending node (deg).  Nc = rate of change in deg/day.
    float i0, ic;           // inclination to the ecliptic (deg).
    float w0, wc;           // argument of perihelion (deg).
    float a0, ac;           // semi-major axis, or mean distance from Sun (AU).
    float e0, ec;           // eccentricity (0=circle, 0..1=ellipse, 1=parabola).
    float M0, Mc;           // M0 = mean anomaly (deg) (0 at perihelion; increases uniformly with time).  Mc ("mean motion") = rate of change in deg/day = 360/period.
    float radius;           // radius proportional to earth's radius (earth = 1.0)
} planet_t;

// utility functions

void sunEclipticCartesianCoordinates ( cartesian_coordinates_t * sun );
void planetEclipticCartesianCoordinates ( cartesian_coordinates_t * location, const planet_t * planet );

float eccentricAnomaly (float e, float M);

void addCartesianCoordinates ( cartesian_coordinates_t * base, const cartesian_coordinates_t * addend );
float rev (float x);

#ifdef __cplusplus
}
#endif

#endif  /* _PLANET_MOTION_H  */
