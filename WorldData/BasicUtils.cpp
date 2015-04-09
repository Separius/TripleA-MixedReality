/*
 * Written by Juliana Fajardini <jufajardini@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */



#include "BasicUtils.h"
#include "fstream"
#include "WorldData.h"
#define p 3.14
using namespace std;
/* Returns the sum of the two given vectors
 * i.e. v1 + v2
 */
//***************************************************************************************************     was wrote by me :)
/*point::point(double x1,double y1)
{
        x = x1;
        y = y1;
}
point::point(vector_t path)
{
        x = sin(path.angle) * (double)path.length;
        y = cos(path.angle) * (double)path.length;
}
point operator+(point point1,point point2)
{
        point retPoint(0,0);
        retPoint.x = point1.x + point2.x;
        retPoint.y = point1.y + point2.y;
        return retPoint;
}
point operator-(point point1,point point2)
{
        point retPoint(0,0);
        retPoint.x = point1.x - point2.x;
        retPoint.y = point1.y - point2.y;
        return retPoint;
}
void  point::sum(point point1,point point2)
{
        x = point1.x + point2.x;
        y = point1.y + point2.y;
}
void point::subtract(point point1,point point2)
{
        x = point1.x - point2.x;
        y = point1.y - point2.y;
}
double point::distance(point point1,point point2)
{
       double dist = sqrt(pow(point1.x - point2.x,2) + pow(point1.y - point2.y,2));
       return dist;
}
vector_t point::conv2polar(point myPoint,vector_t center)
{
        ofstream LOG ("//home//pooya//mix-real//mr-soccer//soccer//soccer_client//log2.txt");
        vector_t vectorToCenter = center; //vectore ma be center
        
        point cartCenterPoint(0,0);
        cartCenterPoint.x = cos(center.angle) * (double)center.length;
        cartCenterPoint.y = sin(center.angle) * (double)center.length;
        
        point myPointBaseMainCenter(0,0);
        myPointBaseMainCenter.x = cartCenterPoint.x + myPoint.x;
        myPointBaseMainCenter.y = cartCenterPoint.y + myPoint.y;
        
        point me(0,0);
        double dist = distance(me,myPointBaseMainCenter);
        double angle = atan(myPointBaseMainCenter.y / myPointBaseMainCenter.x);
        
        vector_t newPath;
        newPath.angle = angle;
        newPath.length = dist;
        
        return newPath;
        /*vector_t centerVector = center; //bordare ma be noghteye 0 o 0 vaghei ro moshakhas mikoe
        point centerPoint(0,0); //mokhtasate khodemoon(dar asl 0 o 0 nistim vali dar base injoory tarif shode)
        
        
        LOG << "x of centerPoint:  " << centerPoint.x << endl;
        LOG << "y of centerPoint:  " << centerPoint.y << endl;
        centerPoint.x = sin((center.angle)) * (double)center.length; //x noghteye 0 o 0 ro darim peyda mikonim
        centerPoint.y = cos(center.angle) * (double)center.length; //y noghteye 0 o 0 ro darim peyda mikonim
        LOG << "x of main center(0):   " << centerPoint.x << endl;
        LOG << "y of main center(0):  " << centerPoint.y << endl;
        
        point newPoint(0,0); //nogheye ma bar hasbe markaze 0 o 0 vaghei(markaze decarti)
        
        
        newPoint.x = centerPoint.x - myPoint.x; //x noghteye ma
        newPoint.y = centerPoint.y - myPoint.y; //y noghteye ma

        LOG << "x of my point base of 0:  " << newPoint.x << endl;
        LOG << "y of my point base of 0:  " << newPoint.y << endl;
        
        point me(0,0); //khodemoon 
        
        
        double dist = me.distance(me,newPoint); //faseleye ma az nogheye ma bar hasbe 0 o 0
        double angle;
        if(newPoint.x >= 0 && newPoint.y >= 0)
                angle = atan(newPoint.y / newPoint.x);// * 180) / p;
        if(newPoint.x >= 0 && newPoint.y < 0)
                angle = atan(newPoint.y / newPoint.x);// * 180) / p;
        if(newPoint.x < 0 && newPoint.y > 0)
                angle = (2 * 3.14) - fabs(atan(newPoint.y / newPoint.x)); //* 180) / p);
        if(newPoint.x < 0 && newPoint.y < 0)
                angle = (-2 * 3.14) + atan(newPoint.y / newPoint.x);// * 180) / p);
        //double angle = atan(newPoint.y / newPoint.x); //zavie ba """"""""""""""""""
        
        
        vector_t polarVec; 
        polarVec.length = dist;
        polarVec.angle = angle;
        return polarVec;*/
//}  */

/*point
subtract(point point1,point point2)
{
        double x = point1.x - point2.x;
        double y = point1.y - point2.y;
        point newPoint;
        newPoint.x = x;
        newPoint.y = y;
        return newPoint;
}

double distance(point point1,point point2)
{
        float dist = sqrt(pow(point1.x - point2.x,2) + pow(point2.y - point2.y,2));
        return dist;
}

vector_t 
conv2polar(point point)
{
     double angle = Atan(point.y / point.x);
     double dist = distance
}*/
/*********************************************************************************************/
vector_t
sum(vector_t v1, vector_t v2)
{
    /* Transform to Cartesian coordinates
     */
    double x1 = sin(v1.angle) * (double)v1.length;
    double y1 = cos(v1.angle) * (double)v1.length;  //i changed them together
    
    
    
    
    double x2 = sin(v2.angle) * (double)v2.length;
    double y2 = cos(v2.angle) * (double)v2.length;
    /* Sum
     */
    x1 += x2;
    y1 += y2;
    /* Transform back to polar coordinates
     */
    v1.angle = atan2(x1,y1);
    v1.length = sqrt(x1*x1+y1*y1);
    /* Length should be always positive so
     * if negative then we change direction
     * instead
     */
    POS_LEN(v1)
    /* Adjust the angle to be between -180
     * and 180 degrees.
     */
    BOUND_ANGLE(v1.angle)
    return v1;
}

vector_t
makeReverse(vector_t vector)
{
    vector_t newVector;
    newVector.length = vector.length;
    
    
    float a = vector.angle;
    a = (a * 180) / 3.14;    //convert from radian to degree
    
    if(a >= 0)
        a -= 180;
    else                      //make mokammel!
        a += 180;    
    
    /*if(a >= 0)
        a = a - 180;
    else                                 //make mokammel!
        a = 180 + a;*/
        
    a = (a * 3.14) / 180;    //convert from degree to darian
    
    newVector.angle = a;
    return newVector;
    
}

/* Returns the difference of the two given vectors
 * i.e. v1 - v2
 */
vector_t
subtract(vector_t v1, vector_t v2)
{
    /* Transform to Cartesian coordinates
     */
    double x1 = sin(v1.angle) * (double)v1.length;
    double y1 = cos(v1.angle) * (double)v1.length;
    double x2 = sin(v2.angle) * (double)v2.length;
    double y2 = cos(v2.angle) * (double)v2.length;
    /* Subtract
     */
    x1 -= x2;
    y1 -= y2;
    /* Transform back to polar coordinates
     */
    v1.angle = atan2(x1,y1);
    v1.length = sqrt(x1*x1+y1*y1);
    /* Length should be always positive so
     * if negative then we change direction
     * instead
     */
    POS_LEN(v1)
    /* Adjust the angle to be between -180
     * and 180 degrees.
     */
    BOUND_ANGLE(v1.angle)
    return v1;
}

/* Scales the vector (multiply length times scalar)
 */
vector_t
scale(vector_t v, double scalar)
{
    /* Scale length
     */
    v.length *= scalar;
    /* Length should be always positive so
     * if negative then we change direction
     * instead
     */
    POS_LEN(v)
    return v;
}

/* Adjust the length of the vector to 1 unit
 * i.e. v / |v|
 */
vector_t
unit(vector_t v)
{
    POS_LEN(v)
    v.length = 1.0;
    return v;
}
