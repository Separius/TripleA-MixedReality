/*
 * Agent.cpp
 *
 *  Created on: Apr 27, 2009
 *      Author: fagner
 */

#include "Agent.h"

bool operator<(const Agent& lhs, const Agent& rhs)
{
	return lhs.uid() < rhs.uid();
}

bool operator==(const Agent& lhs, const Agent& rhs)
{
	return lhs.uid() == rhs.uid();
}

bool operator!=(const Agent& lhs, const Agent& rhs)
{
	return !(lhs == rhs);
}

std::ostream& operator<< (std::ostream& os, const Agent& a)
{
	os << "Agent uid=" << a.uid() << " "
	   << " nick=" << a.nickname() << " "
	   << "found=" << (a.found() ? "yes" : "no")  << " "
	   << (a.side() == side_TEAMMATE ? "teammate" : "opponent");

	if (a.found())
	{
		os << "("
		   << a.position().position.length << "mm, "
		   << a.position().position.angle << "º, "
		   << a.position().orientation << "orient."
		   << ")";
	}

	return os;
}
