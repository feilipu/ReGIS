
#include <stdint.h>
#include <math.h>

#include "planet_motion.h"

void sunEclipticCartesianCoordinates ( cartesian_coordinates_t * sun )
{
    // We use formulas for finding the Sun as seen from Earth, 
    // then negate the (x,y,z) coordinates obtained to get the Earth's position 
    // from the Sun's perspective.

    // http://www.astro.uio.no/~bgranslo/aares/calculate.html
    // http://www.meteorobs.org/maillist/msg09197.html              <== Correct formulas, more accurate (complex)

    // These formulas use 'd' based on days since 1/Jan/2000 12:00 UTC ("J2000.0"), instead of 0/Jan/2000 0:00 UTC ("day value").
    // Correct by subtracting 1.5 days...
    float T = (sun->day - 1.5) * 0.0000273785;                      // 36525.0 Julian centuries since J2000.0

    float T_sqr = sqr(T);

    float L0 = rev(280.46645 + (36000.76983 * T) + (0.0003032 * T_sqr));                            // Sun's mean longitude, in degrees
    float M0 = rev(357.52910 + (35999.05030 * T) - (0.0001559 * T_sqr) - (0.00000048 * T * T_sqr));     // Sun's mean anomaly, in degrees

                                                                    // Sun's equation of center in degrees
    float C = rev((1.914600 - 0.004817 * T - 0.000014 * T_sqr) * sin(RAD(M0)) + (0.01993 - 0.000101 * T) * sin(RAD(2*M0)) + 0.000290 * sin(RAD(3*M0)));

    float LS = rev(L0 + C);                                         // true ecliptical longitude of Sun

    float e = 0.016708617 - T * (0.000042037 + T * 0.0000001236);   // The eccentricity of the Earth's orbit.
    float distanceInAU = (1.000001018 * (1 - sqr(e))) / (1 + e * cos(RAD(M0 + C))); // distance from Sun to Earth in astronomical units (AU)
    sun->x = distanceInAU * cos(RAD(LS));
    sun->y = distanceInAU * sin(RAD(LS));
    sun->z = 0.0;                                                   // the Earth's center is always on the plane of the ecliptic (z=0), by definition!

    sun->au = distanceInAU;
}


void planetEclipticCartesianCoordinates ( cartesian_coordinates_t * location, const planet_t * planet )
{
    float day = location->day;

    float N = rev( planet->N0 + (day * planet->Nc) );
    float i = rev( planet->i0 + (day * planet->ic) );
    float w = rev( planet->w0 + (day * planet->wc) );
    float a = rev( planet->a0 + (day * planet->ac) );

    float e = rev( planet->e0 + (day * planet->ec) );
    float M = rev( planet->M0 + (day * planet->Mc) );

    float E = rev(eccentricAnomaly (e, M));

    // Calculate the body's position in its own orbital plane, and its distance from the thing it is orbiting.
    float xv = a * (cos(RAD(E)) - e);
    float yv = a * sqrt(1.0 - sqr(e)) * sin(RAD(E));

    float v = DEG(atan2(yv, xv));       // True anomaly in degrees: the angle from perihelion of the body as seen by the Sun.
    float r = hypot(xv, yv);            // Distance from the Sun to the planet in AU

    float cosN  = cos(RAD(N));
    float sinN  = sin(RAD(N));
    float cosi  = cos(RAD(i));
    float sini  = sin(RAD(i));
    float cosVW = cos(RAD(v+w));
    float sinVW = sin(RAD(v+w));

    // Now we are ready to calculate (unperturbed) ecliptic cartesian heliocentric coordinates.
    location->x = r * (cosN*cosVW - sinN*sinVW*cosi);
    location->y = r * (sinN*cosVW + cosN*sinVW*cosi);
    location->z = r * sinVW * sini;

    // save the radius from the sun in AU
    location->au = r;
}


float eccentricAnomaly (float e, float M)
{
    float E, error;

    E = M + (e * sin(RAD(M)) * (1.0 + (e * cos(RAD(M)))));

    do {
        error = (E - DEG(e * sin(RAD(E))) - M) / (1 - e * cos(RAD(E)));
        E -= error;
        error = fabs(error);
    } while (error >= 1.0e-3);          // the angle is good enough for our purposes

    return E;
}

void addCartesianCoordinates ( cartesian_coordinates_t * base, const cartesian_coordinates_t * addend )
{
    base->x += addend->x;
    base->y += addend->y;
    base->z += addend->z;
}


float rev (float x)
{
    return x - floor(x*(1/360.0))*360.0;
}


