/*
 *  Copyright 2002-2009, Eskilas Team, Allameh Helli High School (NODET).
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
 *  This file is created by: Pooria Kaviani
 *  and is modified by: Mehrdad Bakhtiari
 *
 *  Released on Monday 1 August 2005, 10 Mordad 1384 by Simorgh RoboCup Team.
 *  For more information please read README file.
*/

#ifndef __RECTANGLE__H
#define __RECTANGLE__H

#include <Types.h>
#include <Line.h>

class Rectangle
{
protected:
	Point firstPoint;
	Point secondPoint;
	Point center;

	double length;
	double width;
public:
	Rectangle();
	Rectangle(Point a, Point b);
	Rectangle(double x1, double y1, double x2, double y2);

	Rectangle assignFromCenter(Point c, double len, double wid);

	Rectangle& operator+=(Rectangle rect);
	Rectangle& operator-=(Rectangle rect);

	bool isInRectangle(Point a) const;

	void setFirstPoint(Point a);
	void setSecondPoint(Point a);

	Point getFirstPoint() const;//topLeft
	Point getSecondPoint() const;//bottomRight

	Point getCenter();

	double getWidth() const;
	double getLength() const;

	Line getLeftEdge() const;
	Line getRightEdge() const;

	Point getTopRightPoint() const;
	Point getBottomLeftPoint() const;
	Point getTopLeftPoint() const;
	Point getBottomRightPoint() const;
};

#endif // __RECTANGLE_H
