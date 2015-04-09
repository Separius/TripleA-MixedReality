/*
 * Defines macros and/ or data types used in more than one hear/source file,
 * such as vector_t.
 * They're more like "global tools".
 *
 * Some or most of the above are reused things from first pv-soccer-client,
 * written by Rodrigo Guerra, but it's posible to take advantage of this
 * structure for future generic/global utilities/tools.
 *
 */

#ifndef BASICUTILS_H_
#define BASICUTILS_H_

#include <cmath>
//#include "WorldData.h"
//using namespace std;
typedef enum _mrteam{TEAM_BLUE = 1, TEAM_YELLOW}mrTeam;

/* This defines a variable type for defining
 * polar vectors.
 */
typedef struct vector_t__ {
    double length;
    double angle;
} vector_t;

//       was wrote by me :)     

/*typedef struct point_p__ {
        double x;
        double y;
} point_p;*/
/*class point
{
        public:
        point(double x1,double y1);
        point(vector_t path);
        double x;
        double y;
        void sum(point point1,point point2);
        void subtract(point point1,point point2);
        double distance(point point1,point point2);
        vector_t conv2polar(point point,vector_t center);

};*/


vector_t
basedOnCenterFlag(vector_t vector);

vector_t
makeReverse(vector_t vector);

/*point_p
sum(point_p point1,point_p point2);

point_p
subtract(point_p point1,point_p point2);

double
distance(point_p point1,point_p point2);

vector_t
conv2polar(point_p point);*/

//////////////////////////////////////////////////
/*
 * Server also sends agent's orientation.
 */
typedef struct agentVector_{
	agentVector_() : position(), orientation() {}
    vector_t position;
    int orientation;
} agentVector;

/* This macro adjust the length of a vector to be always
 * positive by inverting the direction (angle plus 180 degrees)
 * if necessary
 */
#define POS_LEN(v) { if ((v).length < 0) { (v).length *= -1.0 ; (v).angle += M_PI;} }
/* This macro makes sure the angle is between -pi and pi.
 * Caution! The angle is supposed to be within reasonable
 * bounds. If the value is too big this will cause delay!
 */
#define BOUND_ANGLE(a) {while ((a)>(M_PI)) (a)-=(2*M_PI);while ((a)<(-M_PI)) (a)+=(2*M_PI);}

/* Converts a value from degrees to radians
 */
#define DEG2RAD(x) ((x)*M_PI/180.0)


/* Returns the sum of the two given vectors
 * i.e. v1 + v2
 */
vector_t
sum(vector_t v1, vector_t v2);

/* Returns the difference of the two given vectors
 * i.e. v1 - v2
 */
vector_t
subtract(vector_t v1, vector_t v2);

/* Scales the vector (multiply length times scalar)
 */
vector_t
scale(vector_t v, double scalar);

/* Adjust the length of the vector to 1 unit
 * i.e. v / |v|
 */
vector_t
unit(vector_t v);

#endif /*BASICUTILS_H_*/
