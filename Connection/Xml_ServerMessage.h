/*
 * Written by Juliana Fajardini <jufajardini@gmail.com>,
 * basead on soccer server xml classes, made by
 * Adailton Cerqueira <adailton.junior@gmail.com>,
 * Elen Casaes <elenmargareth@gmail.com>
 * and Jose Grimaldo da Silva <jose.jgrimaldo@gmail.com>
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


#ifndef XML_SERVERMESSAGE_H_
#define XML_SERVERMESSAGE_H_

#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include <DeluxeML/XMLSerializable.h>
#include <DeluxeML/XMLSerializer.h>
#include <DeluxeML/XMLItem.h>

#include "AgentList.h"
#include "Agent.h"
#include "BasicUtils.h"
#include "Xml_CommandInterface.h"


class BallInfo : public XMLSerializable
{
public:
	BallInfo();

    virtual bool const write(std::ostream* doc, unsigned int indent=0);
    virtual bool const read(const TLDocumentSlice& slice);
    virtual std::string const getName() const { return "ball"; }

public:
    unsigned int mDist;
    float mAngle;
};

std::ostream& operator<< (std::ostream& os, BallInfo& b);


class TeamMate : public XMLSerializable
{
public:
	TeamMate();
    virtual bool const write(std::ostream* doc, unsigned int indent=0);
    virtual bool const read(const TLDocumentSlice& slice);
    virtual std::string const getName() const { return "teamMate"; }

public:
    unsigned int mId;
    std::string mNickname;
    bool mStatus;
    unsigned int mDist;
    double mAngle;
    unsigned int mOrientation;
};

std::ostream& operator<< (std::ostream& os, TeamMate& tm);

class Opponent : public XMLSerializable
{
public:
	Opponent();
    virtual bool const write(std::ostream* doc, unsigned int indent=0);
    virtual bool const read(const TLDocumentSlice& slice);
    virtual std::string const getName() const { return "opponent"; }

public:
    unsigned int mId;
    std::string mNickname;
    bool mStatus;
    unsigned int mDist;
    double mAngle;
    unsigned int mOrientation;
};

std::ostream& operator<< (std::ostream& os, Opponent& opp);

class Flag : public XMLSerializable
{
public:
	Flag();

    virtual bool const write(std::ostream* doc, unsigned int indent=0);
    virtual bool const read(const TLDocumentSlice& slice);
    virtual std::string const getName() const { return "flag"; }
public:
    std :: string mId;
    unsigned int mDist;
    float mAngle;
};

std::ostream& operator<< (std::ostream& os, const Flag& f);

class Score : public XMLSerializable
{
public:
	Score();

    virtual bool const write(std::ostream* doc, unsigned int indent=0);
    virtual bool const read(const TLDocumentSlice& slice);
    virtual std::string const getName() const { return "score"; }
public:
    int mYellow, mBlue;
};

std::ostream& operator<< (std::ostream& os, const Score& s);

class WorldDataApplication : public XMLSerializable
{
public:

    virtual bool const write(std::ostream* doc, unsigned int indent=0);
    virtual bool const read(const TLDocumentSlice& slice);
    virtual std::string const getName() const { return "worldData"; }

public:
	WorldDataApplication();

	AgentList Teammates() {
		return mOthers.From(side_TEAMMATE);
	}

	AgentList Opponents() {
		return mOthers.From(side_OPPONENT);
	}

	AgentList All() {
		return mOthers;
	}

	Agent     Me() {
		return mMe;
	}

    Agent getAgent(int uid);

    /*
     * Returns Flag given a certain id.
     * */
    Flag getFlag(const std::string& id);

    /* Returns vector_t structure corresponding to
     * the given flag.
     * */
    vector_t getVector(Flag flag);

    /*
     * Returns a bool telling last agent status, sent
     * by last update from the soccer server.
     */
    bool isAgentFound();
    bool isAgentFound(int uid);

public:

	Agent     	  mMe;
	AgentList 	  mOthers;
    double 		  mTime;
    int mMaxAgents;
    std::string mPlayMode;
    Score mScore;
    CommandType mLastCommand;
    BallInfo mBall;
    std::vector<Flag> mFlags;

private:

	std::string   mNickname;
    unsigned int  mAgentID;
    bool          mStatus;
};

class Comp {
public:
	bool operator() (const Flag& f1, const Flag& f2)
	{
		return f1.mId < f2.mId;
	}
};

inline std::ostream& operator<<(std::ostream& os, WorldDataApplication& wda)
{
	os << "WorldData: time(" << wda.mTime << ") "
	   << "me=[" << wda.Me() << "] "
	   << "max_agents=" << wda.mMaxAgents << " "
	   << "play_mode=" << wda.mPlayMode <<  " "
	   << wda.mScore << " ";

	if (wda.Me().found())
	{
		os << wda.mBall << " ";

		std::vector<Flag> flags = wda.mFlags;
		std::sort(flags.begin(), flags.end(), Comp());

		std::vector<Flag>::iterator fl_it = flags.begin();
		for (; fl_it != flags.end(); ++fl_it)
		{
			os << "[" << (*fl_it) << "] ";
		}

		os << "\n";
		AgentList ags = wda.All();
		std::vector<Agent>::iterator ag_it = ags.begin();
		for (; ag_it != ags.end(); ++ag_it)
		{
			os << "[" << (*ag_it) << "] ";
		}

		os << "\n";
	}

	os << std::flush;

	return os;
}


#endif /*XML_SERVERMESSAGE_H_*/
