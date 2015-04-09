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

#ifndef __VECTOR_H
#define __VECTOR_H

#include <iostream>

#include <Types.h>
#include <vector_2d.h>

#ifndef NULL
#	define NULL 0
#endif // NULL

class Vector
{	
public:
        float x, y;
        float magnitude;
	float direction;
	Vector(){}
	Vector(const Point &point);
	Vector(const rcsc::Vector2D &point);
	Vector(float x, float y);
	Vector(float x, float y, bool a);

	Vector& operator+=(const Vector &Vector1);
	Vector& operator-=(const Vector &Vector1);
	Vector& operator*=(const float Number);
	Vector& operator/=(const float Number);

	operator Point() const;
      
	void rotate(float rotateDir);

	void setAsCartesian(float xArg, float yArg);

	void setAsPolar(float magnitudeArg, float directionArg);

	void setByDistDirChange(Vector &headVector, float distChange, float dirChange);

	void setByPoints(Point startPoint, Point endPoint);

	void setLength(float newX)
	{
		x = newX;
	};

	// Transforming functions
	Point asPoint() const;

	// Getting functions
	float getX() const;

	float getY() const;

	float getAbsY() const;

	float getMagnitude() const;

	float getDirection() const;

	void normalize();

	float getDistance(const Vector vec) const;

	float getAngles(Vector v1,Vector v2);
	
	float getSumAngles(Vector v1,Vector v2,Vector v3);
};	

Vector operator+(const Vector &Vector1, const Vector &Vector2);
Vector operator-(const Vector &Vector1, const Vector &Vector2);
Vector operator*(const Vector &Vector1, const float magFactor);
Vector operator/(const Vector &Vector1, const float magFactor);
Vector operator+(const Vector &Vector1); // Sassan
Vector operator-(const Vector &Vector1); // Sassan
std::ostream &operator<<(std::ostream &stream, const Vector &vector);

#endif // __VECTOR_H
