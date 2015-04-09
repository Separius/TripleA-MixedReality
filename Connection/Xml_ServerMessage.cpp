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

#include "ClientException.h"
#include "Xml_ServerMessage.h"
#include <cassert>

WorldDataApplication::WorldDataApplication()
	: mTime(), mMaxAgents(), mAgentID(-1), mStatus()
{

}

BallInfo::BallInfo()
	: mDist(0), mAngle(0)
{

}

bool const
BallInfo::write(std::ostream* doc, unsigned int indent)
{
    XMLSerializer::writeLeafNode(doc, "dist", mDist, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "angle", mAngle, NULL, indent);
    return true;
}

bool const
BallInfo::read(const TLDocumentSlice& slice)
{
    vector<XMLItem> items;
    XMLSerializer::itemize(slice, items);

    vector<XMLItem>::iterator it = items.begin();

    for (; it != items.end(); ++it)
    {
    	if (it->getName() == "dist")
    	{
    		mDist = it->getContent_int();
    	}
    	else if (it->getName() == "angle")
    	{
    		mAngle = DEG2RAD(it->getContent_float());
    	}
    }

    return true;
}

std::ostream& operator<< (std::ostream& os, BallInfo& b)
{
	os << "Ball dist=" << b.mDist << ", angle=" << b.mAngle;
	return os;
}

TeamMate::TeamMate()
	: mId(-1), mNickname("-"), mStatus("-"),
	  mDist(0), mAngle(0), mOrientation(0)
{

}

bool const
TeamMate::write(std::ostream* doc, unsigned int indent)
{
    XMLSerializer::writeLeafNode(doc, "id", mId, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "nickname", mNickname, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "status", mStatus, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "dist", mDist, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "angle", mAngle, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "orientation", mOrientation, NULL, indent);

    return true;
}

bool const
TeamMate::read(const TLDocumentSlice& slice)
{
    vector<XMLItem> items;
    XMLSerializer::itemize(slice, items);

    vector<XMLItem>::iterator it;
    for (it = items.begin(); it != items.end(); ++it)
    {
    	if (it->getName() == "id")
    	{
            mId = it->getContent_int();
    	}
    	else if (it->getName() == "nickname")
    	{
            mNickname = it->getContent();
    	}
    	else if (it->getName() == "status")
    	{
            mStatus = (it->getContent() == "found" ? true : false);
    	}
    	else if (it->getName() == "dist")
    	{
            mDist = it->getContent_int();
    	}
    	else if (it->getName() == "angle")
    	{
            mAngle = DEG2RAD(it->getContent_int());
    	}
    	else if (it->getName() == "orientation")
    	{
            mOrientation = DEG2RAD(it->getContent_int());
    	}
    }

    return true;
}

std::ostream& operator<< (std::ostream& os, TeamMate& tm)
{
	os << "teammate(" << tm.mId << ", " << tm.mNickname << ") ";

	if (tm.mStatus)
	{
		os << "at dist=" << tm.mDist
		   << ", angle=" << tm.mAngle
		   << " and " << tm.mOrientation << "º of you";
	}
	else
	{
		os << "was not found";
	}

	return os;
}

Opponent::Opponent()
	: mId(-1),
	  mNickname("-"),
	  mStatus("-"),
	  mDist(0),
	  mAngle(0),
	  mOrientation(0)
{

}

bool const
Opponent::write(std::ostream* doc, unsigned int indent)
{
    XMLSerializer::writeLeafNode(doc, "id", mId, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "nickname", mNickname, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "status", mStatus, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "dist", mDist, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "angle", mAngle, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "orientation", mOrientation, NULL, indent);

    return true;
}


bool const
Opponent::read(const TLDocumentSlice& slice)
{
    vector<XMLItem> items;
    XMLSerializer::itemize(slice, items);

    vector<XMLItem>::iterator it;
    for (it = items.begin(); it != items.end(); ++it)
    {
    	if (it->getName() == "id")
    	{
            mId = it->getContent_int();
    	}
    	else if (it->getName() == "nickname")
    	{
            mNickname = it->getContent();
    	}
    	else if (it->getName() == "status")
    	{
            mStatus = (it->getContent() == "found" ? true : false);
    	}
    	else if (it->getName() == "dist")
    	{
            mDist = it->getContent_int();
    	}
    	else if (it->getName() == "angle")
    	{
            mAngle = DEG2RAD(it->getContent_int());
    	}
    	else if (it->getName() == "orientation")
    	{
            mOrientation = DEG2RAD(it->getContent_int());
    	}
    }

    return true;
}

std::ostream& operator<< (std::ostream& os, Opponent& opp)
{
	os << "opponent(" << opp.mId << ", " << opp.mNickname << ") ";

	if (opp.mStatus)
	{
		os << "at dist=" << opp.mDist
		   << ", angle=" << opp.mAngle
		   << " and " << opp.mOrientation << "º of you";
	}
	else
	{
		os << "was not found";
	}

	return os;
}

Flag::Flag()
	: mId(""), mDist(0), mAngle(0)
{

}

bool const
Flag::write(std::ostream* doc, unsigned int indent)
{
    XMLSerializer::writeLeafNode(doc, "id", mId, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "dist", mDist, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "angle", mAngle, NULL, indent);
    return true;
}

bool const
Flag::read(const TLDocumentSlice& slice)
{
    vector<XMLItem> items;
    XMLSerializer::itemize(slice, items);

    vector<XMLItem>::iterator it;
    for (it = items.begin(); it != items.end(); ++it)
    {

        if (it->getName() == "id")
        {
        	mId = it->getContent();
        }
        else if (it->getName() == "dist")
        {
        	mDist = (int) it->getContent_float();
        }
        else if (it->getName() == "angle")
        {
        	mAngle = DEG2RAD(it->getContent_float());
        }
    }

    return true;
}

std::ostream& operator<< (std::ostream& os, const Flag& f)
{
	os << "Flag named " << f.mId
	   << " dist=" << f.mDist << " and "
	   << f.mAngle << "º from you";
	return os;
}

Score::Score()
	: mYellow(-1), mBlue(-1)
{

}

bool const
Score::write(std::ostream* doc, unsigned int indent)
{
    XMLSerializer::writeLeafNode(doc, "yellow", mYellow, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "blue", mBlue, NULL, indent);

    return true;
}

bool const
Score::read(const TLDocumentSlice& slice)
{
    vector<XMLItem> items;
    XMLSerializer::itemize(slice, items);

    vector<XMLItem>::iterator it;
	for (it = items.begin(); it != items.end(); ++it)
	{
		if (it->getName() == "yellow")
		{
		    mYellow = items[0].getContent_int();
		}
		else if (it->getName() == "blue")
		{
			mBlue = items[1].getContent_int();
		}
	}

    return true;
}

std::ostream& operator<< (std::ostream& os, const Score& s)
{
	os << "Score(blue= " << s.mBlue << ", yellow " << s.mYellow << ")";
	return os;
}

bool const
WorldDataApplication::write(std::ostream* doc, unsigned int indent)
{
#if 0
    XMLSerializer::writeLeafNode(doc, "time", mTime, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "agent_id", mAgentID, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "status", mStatus, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "max_agent", mMaxAgents, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "playMode", mPlayMode, NULL, indent);
    XMLSerializer::writeNestedNode(doc, mScore, indent);
    XMLSerializer::writeNestedNode(doc, mLastCommand, indent);
    XMLSerializer::writeNestedNode(doc, mBall, indent);

    /* Writing TeamMates, Opponents and Flags vectors. */
    vector<Agent>::iterator tm_it;
    for ( tm_it = mOthers.begin(); tm_it != mOthers.end(); ++tm_it)
    {
    	if (tm_it->side() == side_TEAMMATE)
    	{
    		TeamMate tm;
    		tm.mAngle =
    		XMLSerializer::writeNestedNode(doc, *tm_it, indent);
    	}
    	else
    	{

    	}
    }

    vector<Flag>::iterator fl_it;
    for (fl_it = mFlags.begin(); fl_it != mFlags.end(); ++fl_it)
    {
        XMLSerializer::writeNestedNode(doc, *fl_it, indent);
    }
#endif
    return true;
}

bool const
WorldDataApplication::read(const TLDocumentSlice& slice)
{
    vector<XMLItem> items;
    XMLSerializer::itemize(slice, items);

    for (unsigned int i = 0 ; i < items.size() ; ++i)
    {
	    if (items[i].getItemType() == XML_NESTED)
	    {
		    if (items[i].getName() == "kick" ||
			    items[i].getName() == "wheel_velocity" ||
			    items[i].getName() == "goto")
		    {
		    	mLastCommand.read(items[i]);
		    }
		    else if (items[i].getName() == "score")
		    {
			    mScore.read(items[i]);
		    }
		    else if (items[i].getName() == "ball")
		    {
		    	mBall.read(items[i]);
		    }
		    else if (items[i].getName() == "flag")
		    {
		    	Flag aux_flag;
		    	aux_flag.read(items[i]);
				mFlags.push_back(aux_flag);
		    }
		    else if (items[i].getName() == "teamMate")
		    {
		    	TeamMate aux;
		    	aux.read(items[i]);
		    	//mTeamMates.push_back(aux_mate);

		    	Agent a;

		    	if (aux.mStatus)
		    	{
		    		agentVector v;
		    		v.orientation = aux.mOrientation;
		    		v.position.length = aux.mDist;
		    		v.position.angle  = aux.mAngle;

		    		a = Agent(side_TEAMMATE, aux.mId,
		    			aux.mNickname, v);
		    	}
		    	else
		    	{
		    		a = Agent(side_TEAMMATE, aux.mId,
		    			aux.mNickname);
		    	}



		    	mOthers.push_back(a);

		    }
		    else if (items[i].getName() == "opponent")
		    {
		    	Opponent aux;
		    	aux.read(items[i]);
		    	//mOpponents.push_back(aux_opponent);

		    	Agent a;

		    	if (aux.mStatus)
		    	{
		    		agentVector v;
		    		v.orientation = aux.mOrientation;
		    		v.position.length = aux.mDist;
		    		v.position.angle  = aux.mAngle;

		    		a = Agent(side_OPPONENT, aux.mId,
		    			aux.mNickname, v);
		    	}
		    	else
		    	{
		    		a = Agent(side_OPPONENT, aux.mId,
		    			aux.mNickname);
		    	}

		    	mOthers.push_back(a);
		    }
	    }
	    else if (items[i].getName() == "time")
	    {
		    mTime = items[i].getContent_double();
	    }
	    else if (items[i].getName() == "agent_id")
	    {
		    mAgentID = items[i].getContent_int();
	    }
	    else if (items[i].getName() == "status")
	    {
		    mStatus = (items[i].getContent() == "found" ? true : false);
	    }
	    else if (items[i].getName() == "max_agent")
	    {
		    mMaxAgents = items[i].getContent_int();
	    }
	    else if (items[i].getName() == "playMode")
	    {
		    mPlayMode = items[i].getContent();
	    }
	    else if (items[i].getName() == "nickname")
	    {
		    mNickname = items[i].getContent();
	    }
	    else
	    {
	    	std::cout << "Some error occurred while trying to read message"
	    			" received by server." << std::endl;
		    return false;
	    }
    }



    agentVector vec;
    vec.orientation = 0;
    vec.position.angle = 0;
    vec.position.length = 0;


    if (mStatus)
    	mMe = Agent(side_TEAMMATE, mAgentID, mNickname, vec);
    else
    	mMe = Agent(side_TEAMMATE, mAgentID, mNickname);

    return true;
}

/*
 * Returns TeamMate given a certain id.
 * */
Agent WorldDataApplication::getAgent(int uid)
{
    return mOthers.ByUid(uid);
}

/*
 * Returns Flag given a certain id.
 * */
Flag
WorldDataApplication::getFlag(const std::string& id)
{
    for(unsigned i = 0; i < mFlags.size(); i++)
    {
        if(mFlags[i].mId == id)
        {
            return mFlags[i];
        }
    }

    throw ClientException("Logic error in flag request");
}

/* Returns vector_t structure corresponding to
 * the given flag.
 * */
vector_t
WorldDataApplication::getVector(Flag flag)
{
    vector_t aux;
    aux.angle = (double)flag.mAngle;
    aux.length = (double)flag.mDist;

    return aux;
}

/*
 * Tells if server found given agent in last cicle's world update.
 * Doesn't handle with posible invalid ids.
 */
bool
WorldDataApplication :: isAgentFound(int id)
{
    if(getAgent(id).found())
    {
        return true;
    }
    else
    {
    	return false;
    }
}

/*
 * Returns a bool telling last agent status, sent
 * by last update from the soccer server.
 */
bool
WorldDataApplication :: isAgentFound()
{
    return mStatus;
}
