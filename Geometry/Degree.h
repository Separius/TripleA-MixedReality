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

#ifndef __DEGREE_H
#define __DEGREE_H

#include <Types.h>

namespace Degree
{
	extern float normalizeAngle(float angle);
	extern float absoluteAngle(float angle);
	extern bool isBetween(float angle1, float angle2, float checkAngle);
	extern float getDeltaAngle(float angle1,float angle2);

	extern float sin(float direction);
	extern float cos(float direction);
	extern float tan(float direction);
	extern float cot(float direction);
	extern float arcSin(float value);
	extern float arcCos(float value);
	extern float arcTan(float value);
	extern float arcCot(float value);
	extern float arcTan2(float y, float x);
	extern float arcCot2(float y, float x);
	extern float getDegree(Point pos1, Point pos2, Point pos3);
	extern Point turn(Point point, float deg);
}

#endif // __DEGREE_H
