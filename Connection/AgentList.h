/*
 * AgentList.h
 *
 *  Created on: Apr 27, 2009
 *      Author: fagner
 */

#ifndef AGENTLIST_H_
#define AGENTLIST_H_

#include <functional>
#include <algorithm>
#include <cassert>

#include "Agent.h"
#include "BasicUtils.h"
#include "ClientException.h"
#include "util.h"


struct AgentNickFinder {
	AgentNickFinder(const std::string& n) : nick(n) {}

	bool operator()(const Agent& a)
	{ return a.nickname() == nick;	}
private:
	std::string nick;
};

struct AgentUidFinder {
	AgentUidFinder(int u) : uid(u) {}

	bool operator()(const Agent& a)
	{ return a.uid() == uid; }
private:
	int uid;
};

class AgentList {

public:

	typedef std::vector<Agent> AgentCollection;

	typedef AgentCollection::iterator 		iterator;
	typedef AgentCollection::const_iterator const_iterator;
	typedef AgentCollection::size_type      size_type;

public:

	AgentList()
	{

	}

	template <typename Iter>
	AgentList(Iter b, Iter e)
		: mAgents(b, e)
	{
	}

	template <typename Iter>
	AgentList& insert(iterator pos, Iter b, Iter e)
	{
		mAgents.insert(pos, b, e);
		return *this;
	}


	Agent& operator[] (const std::string& nickname)
	{
		AgentList list = findBy(nickname, std::mem_fun_ref(&Agent::nickname));

		if (list.size() == 0) {
			throw ClientException("Requested nickname "
					+ nickname + " don't exist");
		}

		assert(list.size() == 1);
		return *list.begin();
	}

	const Agent& operator[] (const std::string& nickname) const
	{
		AgentList list = findBy(nickname, std::mem_fun_ref(&Agent::nickname));

		if (list.size() == 0) {
			throw ClientException("Requested nickname "
					+ nickname + " don't exist");
		}

		assert(list.size() == 1);
		return *list.begin();
	}

	Agent& ByUid(int uid)
	{
		AgentList list = findBy(uid, std::mem_fun_ref(&Agent::uid));

		if (list.size() == 0) {
			throw ClientException("Requested uid "
					+ convert_to<std::string>(uid)
	 				+ " was not found");
		}

		assert(list.size() == 1);
		return *list.begin();
	}

	const Agent& ByUid(int uid) const
	{
		AgentList list = findBy(uid, std::mem_fun_ref(&Agent::uid));

		if (list.size() == 0) {
			throw ClientException("Requested uid "
					+ convert_to<std::string>(uid)
					+ " was not found");
		}

		assert(list.size() == 1);
		return *list.begin();
	}


	Agent& operator[] (int idx)
	{
		return mAgents[idx];
	}

	const Agent& operator[] (int idx) const
	{
		return mAgents[idx];
	}

	iterator findByNickname(const std::string& nickname)
	{
		return std::find_if(mAgents.begin(), mAgents.end(),
				AgentNickFinder(nickname));
	}

	iterator findByUid(int uid)
	{
		return std::find_if(mAgents.begin(), mAgents.end(),
				AgentUidFinder(uid));
	}

	AgentList From(const AgentSide& side) const
	{
		return findBy(side, std::mem_fun_ref(&Agent::side));
	}

	AgentList WithFound(bool status) const
	{
		return findBy(status, std::mem_fun_ref(&Agent::found));
	}

	iterator begin()
	{
		return mAgents.begin();
	}

	iterator end()
	{
		return mAgents.end();
	}

	const_iterator begin() const
	{
		return mAgents.begin();
	}

	const_iterator end() const
	{
		return mAgents.end();
	}

	void push_back(const Agent& a)
	{
		mAgents.push_back(a);
	}

	iterator erase(iterator it)
	{
		return mAgents.erase(it);
	}

	template <typename Iter>
	void assign(Iter b, Iter e)
	{
		mAgents.assign(b, e);
	}

	bool empty() const
	{
		return mAgents.empty();
	}

	size_type size() const
	{
		return mAgents.size();
	}

	AgentList join(const AgentList& list)
	{
		AgentCollection summed_collection = mAgents;
		summed_collection.insert(end(), list.begin(), list.end());
		AgentList summed(summed_collection.begin(), summed_collection.end());
		return summed;
	}

	std::vector<agentVector> toAgentVector()
	{
		std::vector<agentVector> vectors;

		AgentList agents = WithFound(true);

		AgentCollection::iterator it = agents.begin();
		for (; it != agents.end(); ++it)
		{
			vectors.push_back(it->position());
		}

		return vectors;
	}


	std::vector<vector_t> toVector()
	{
		std::vector<vector_t> vectors;

		AgentList agents = WithFound(true);

		AgentCollection::iterator it = agents.begin();
		for (; it != agents.end(); ++it)
		{
			vectors.push_back(it->position().position);
		}

		return vectors;
	}

private:

	template <typename T, typename Func>
	AgentList findBy(const T& value, Func f)
	{
		AgentList selected_agents;

		AgentList::iterator it;
		for (it = mAgents.begin(); it != mAgents.end(); ++it)
		{
			if (f(*it) == value)
			{
				selected_agents.push_back(*it);
			}
		}

		return selected_agents;
	}

	template <typename T, typename Func>
	AgentList findBy(const T& value, Func f) const
	{
		AgentList selected_agents;

		AgentList::const_iterator it;
		for (it = mAgents.begin(); it != mAgents.end(); ++it)
		{
			if (f(*it) == value)
			{
				selected_agents.push_back(*it);
			}
		}

		return selected_agents;
	}

private:

	AgentCollection mAgents;

};

#endif /* AGENTLIST_H_ */
