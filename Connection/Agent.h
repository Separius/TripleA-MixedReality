/*
 * Agent.h
 *
 *  Created on: Apr 27, 2009
 *      Author: fagner
 */

#ifndef AGENT_H_
#define AGENT_H_

#include <iostream>
#include <string>

#include "BasicUtils.h"
#include "ClientException.h"

typedef enum {
	side_TEAMMATE,
	side_OPPONENT,
} AgentSide;

class Agent {

	static const Agent None;

public:

	Agent()
		: mSide(side_TEAMMATE), mUid(-1), mNickname(), mFound(false)
	{

	}

	Agent(const AgentSide& team, int uid, const std::string& nickname)
		: mSide(team), mUid(uid), mNickname(nickname), mFound(false)
	{

	}

	Agent(const AgentSide& team, int uid,
			const std::string& nickname, const agentVector& av)
		: mSide(team), mUid(uid), mNickname(nickname), mPosition(av), mFound(true)
	{

	}

	bool position(agentVector& p)
	{
		bool was_found = found();

		if (was_found)
		{
			p = mPosition;
		}

		return was_found;
	}

	const agentVector& position() const
	{
		if (!found())
			throw new ClientException("Position request on not found client");

		return mPosition;
	}

	agentVector& position()
	{
		if (!found())
			throw new ClientException("Position request on not found client");

		return mPosition;
	}

	bool found() const
	{
		return mFound;
	}

	std::string nickname() const
	{
		return mNickname;
	}

	int uid() const
	{
		return mUid;
	}

	AgentSide side() const
	{
		return mSide;
	}

private:

	AgentSide    mSide;
	int 		 mUid;
	std::string	 mNickname;
	agentVector  mPosition;
	bool 		 mFound;
};

bool operator<(const Agent& lhs, const Agent& rhs);

bool operator==(const Agent& lhs, const Agent& rhs);

bool operator!=(const Agent& lhs, const Agent& rhs);

std::ostream& operator<< (std::ostream& os, const Agent& a);

#endif /* AGENT_H_ */
