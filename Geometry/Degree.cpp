/*
 *  Copyright 2002-2005, Simorgh Team, Allameh Helli High School (NODET).
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
 *
 *  Released on Monday 1 August 2005, 10 Mordad 1384 by Simorgh RoboCup Team.
 *  For more information please read README file.
*/

#include <cmath>

#include <Line.h>
#include <Degree.h>
#include <Defines.h>
//#include <Logger.h>

using namespace std;
// namespace Degree

namespace Degree
{
	float sin(float direction)
	{
		return ::sin(direction * DEGTORAD);
	}

	float cos(float direction)
	{
		return ::cos(direction * DEGTORAD);
	}

	float tan(float direction)
	{
		return ::tan(direction * DEGTORAD);
	}

	float cot(float direction)
	{
		return 1 / ::tan(direction * DEGTORAD);
	}

	float arcSin(float value)
	{
		return ::asin(value) * RADTODEG;
	}

	float arcCos(float value)
	{
		return ::acos(value) * RADTODEG;
	}

	float arcTan(float value)
	{
		return ::atan(value) * RADTODEG;
	}

	float arcCot(float value)
	{
		return ::atan(1 / value) * RADTODEG;
	}

	float arcTan2(float y, float x)
	{
		return ::atan2(y,x) * RADTODEG;
	}

	float arcCot2(float y, float x)
	{
		return arcTan2(y,x); // Reversing arguments because of Cot.
	}

	float normalizeAngle(float angle)
	{
		while (angle < -180) angle += 360;
		while (angle > 180) angle -= 360;
		return angle;
	}
	
	float absoluteAngle(float angle)
	{
		while (angle < 0) angle += 360;
		while (angle >= 360) angle -= 360;
		return angle;
	}


	bool isBetween(float angle1, float angle2, float checkAngle)
	{
		angle1 = absoluteAngle(angle1);
		angle2 = absoluteAngle(angle2);
		checkAngle = absoluteAngle(checkAngle);

		if (angle1 == angle2)
		{
			if (checkAngle == angle1)
				return true;
			else
				return false;
		}
		else if (angle1 < angle2)
		{
			if (checkAngle >= angle1 && checkAngle <= angle2)
				return true;
			else
				return false;
		}
		else
		{
			if (checkAngle <= angle2 || checkAngle >= angle1)
				return true;
			else
				return false;
		}
	}

	float getDeltaAngle(float angle1,float angle2)
	{
		angle1 = absoluteAngle(angle1);
		angle2 = absoluteAngle(angle2);
		return normalizeAngle(angle1 - angle2);
	}
	
	float getDegree(Point pos1, Point pos2, Point pos3)
	{
		float shib1, shib2, deg1, deg2;
		shib1 = getShib(pos1, pos2);
		shib2 = getShib(pos2, pos3);
		deg1 = atan(shib1) * RADTODEG;
		deg2 = atan(shib2) * RADTODEG;

		if (pos3.x < pos2.x && pos3.y < pos2.y)
			deg2 += 180;
		if (pos3.x > pos2.x && pos3.y < pos2.y)
			deg2 += 360;
		if (pos3.x < pos2.x && pos3.y > pos2.y)
			deg2 += 180;
			
		if (pos1.x < pos2.x && pos1.y < pos2.y)
			deg1 += 180;
		if (pos1.x > pos2.x && pos1.y < pos2.y)
			deg1 += 360;
		if (pos1.x < pos2.x && pos1.y > pos2.y)
			deg1 += 180;

		float degree = deg2 - deg1;
		if (degree > 180)
			degree -= 360;
		else if (degree < -180)
			degree += 360;
		if (degree == 0 && pos1.x < pos2.x && pos2.x < pos3.x)
			degree = 180;
		return degree;
	}
	
	Point turn(Point point, float deg)
	{
		float x = point.x, y = point.y;
		point.x = x * cos(deg) - y * sin(deg);
		point.y = x * sin(deg) + y * cos(deg);
		return point;
	}
}
