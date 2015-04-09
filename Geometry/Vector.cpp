/*
 *  Copyright 2002-2009, Mersad Team, Allameh Helli High School (NODET).
 *
 *  This program is free software, you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Library General Public License for more details.
 *
 *  This file is created by: Ahmad Boorghany
 *  	and is modified by: Mohammad Salehe, Sassan Haradji
 *
 *  Released on Friday 25 September 2009, 3 Mehr 1388 by Mersad RoboCup Team.
 *  For more information please read README file.
*/

#include <cmath>

#include <Vector.h>
#include <Degree.h>
#include <Defines.h>
#include <cassert>

using namespace std;

Vector::Vector(const Point &point)
{
	setAsCartesian(point.x, point.y);
}

Vector::Vector(float x, float y)
{
	setAsCartesian(x, y);
}

Vector::Vector(float x, float y, bool a)
{
	setAsPolar(x, y);
}

Vector::Vector(const rcsc::Vector2D &point)
{
	setAsCartesian(point.x, point.y);
}

Vector operator+(const Vector &Vector1, const Vector &Vector2)
{
	Vector retVector;
	retVector.setAsCartesian(Vector1.getX() + Vector2.getX(),
							 Vector1.getY() + Vector2.getY());
	return retVector;
}

Vector operator-(const Vector &Vector1, const Vector &Vector2)
{
	Vector retVector;
	retVector.setAsCartesian(Vector1.getX() - Vector2.getX(),
							 Vector1.getY() - Vector2.getY());
	return retVector;
}

Vector operator+(const Vector &Vector1) // Sassan
{
	return Vector1;
}

Vector operator-(const Vector &Vector1) // Sassan
{
	Vector retVector;
	retVector.setAsCartesian(-Vector1.getX(), -Vector1.getY());
	return retVector;
}

Vector operator*(const Vector &Vector1, float rate)
{
	Vector retVector;
	retVector.setAsCartesian(Vector1.getX() * rate, Vector1.getY() * rate);
	return retVector;
}


Vector operator/(const Vector &Vector1, float rate)
{
	if (rate)
	{
		Vector retVector;
		retVector.setAsCartesian(Vector1.getX() / rate, Vector1.getY() / rate);
		return retVector;
	}
	else
	{
		cout << "Devision By Zero" << endl;
		assert(0);
		Vector retVector;
		retVector.setAsCartesian(0, 0);
		return retVector;
	}
}


Vector& Vector::operator+=(const Vector &Vector1)
{
	setAsCartesian(x + Vector1.getX(), y + Vector1.getY());
	return *this;
}

Vector& Vector::operator-=(const Vector &Vector1)
{
	setAsCartesian(x - Vector1.getX(), y - Vector1.getY());
	return *this;
}

Vector& Vector::operator*=(const float rate)
{
	setAsCartesian(x * rate, y * rate);
	return *this;
}

Vector& Vector::operator/=(const float rate)
{
	setAsCartesian(x / rate, y / rate);
	return *this;
}

Vector::operator Point() const
{
    return asPoint();
}

ostream &operator<<(ostream &stream, const Vector &vector)
{
	stream << "(" << vector.getX() << "," << vector.getY() << ")";
	return stream;
}

// class Vector

void Vector::setAsCartesian(float xArg, float yArg)
{
	x = xArg;
	y = yArg;

	magnitude = hypot(x, y);
	direction = Degree::normalizeAngle(Degree::arcTan2(y, x));
}

void Vector::setAsPolar(float magnitudeArg, float directionArg)
{
	magnitude = magnitudeArg;
	direction = Degree::normalizeAngle(directionArg);

	x = Degree::cos(direction) * magnitude;
	y = Degree::sin(direction) * magnitude;
}

void Vector::rotate(float rotateDir)
{
	setAsPolar(magnitude, direction + rotateDir);
}

void Vector::setByDistDirChange(Vector &headVector, float distChange, float dirChange)
{
	float erX,erY;
	float vrX,vrY;

	erX = headVector.getX() / headVector.getMagnitude();
	erY = headVector.getY() / headVector.getMagnitude();

	vrX = distChange * erX -
			dirChange * DEGTORAD * headVector.getMagnitude() * erY;
	vrY = distChange * erY +
			dirChange * DEGTORAD * headVector.getMagnitude() * erX;

	setAsCartesian(vrX, vrY);
}

float Vector::getX() const
{
	return x;
}

float Vector::getY() const
{
	return y;
}

float Vector::getAbsY() const
{
	return fabs(y);
}

float Vector::getMagnitude() const
{
	return magnitude;
}

float Vector::getDirection() const
{
	return direction;
}

Point Vector::asPoint() const
{
	return Point(x, y);
}

void Vector::setByPoints(Point startPoint, Point endPoint)
{
	setAsCartesian(endPoint.x - startPoint.x, endPoint.y - startPoint.y);
}

void Vector::normalize()
{
    if (getMagnitude())
        *this /= getMagnitude();
}

float Vector::getDistance(const Vector vec) const
{
	return hypot(x - vec.getX(), y - vec.getY());
}

float Vector::getAngles(Vector v1 , Vector v2)
{
    float dirV1 , dirV2 ;
    dirV1 = v1.getDirection() ;
    dirV2 = v2.getDirection() ;
    if ( ( (dirV1 >= 0) && (dirV2 <= 0) ) || ( (dirV1 <= 0) && (dirV2 >= 0) ) )
	return (fabs ( dirV1 ) + fabs ( dirV2 ) );
    if ( ( (dirV1 >= 0) && (dirV2 >= 0) ) || ( (dirV1 <= 0) && (dirV2 <= 0) ) )
	return ( fabs ( dirV1 - dirV2 ) );
}

float Vector::getSumAngles(Vector v1 , Vector v2 , Vector v3)
{
    float sum12 , sum23 , sum31 ;
    sum12 = getAngles( v1 , v2 );
    sum23 = getAngles( v2 , v3 );
    sum31 = getAngles( v3 , v1 );
    return ( sum12 + sum23 + sum31 );
}