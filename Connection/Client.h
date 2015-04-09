/*
 * Written by Juliana Fajardini <jufajardini@gmail.com>
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

#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "Connection.h"
#include "WorldData.h"
#include "BasicPlayer.h"
#include "Xml_Connection.h"

#include "util.h"
#include "PropertyReader.h"
#include "Properties.h"
#include "Property.h"

class ConnectionParameters {
public:
	ConnectionParameters()
	{

	}

	std::string teamname() const
	{
		return mTeamname;
	}

	void teamname(const std::string& a_teamname)
	{
		mTeamname = a_teamname;
	}

	void nickname(const std::string& a_nickname)
	{
		mNickname = a_nickname;
	}

	std::string nickname() const
	{
		return mNickname;
	}

	int vtId() const
	{
		return mVtId;
	}

	void vtId(int vt_id)
	{
		mVtId = vt_id;
	}

	int rcId() const
	{
		return mRcId;
	}

	void rcId(int rc_id)
	{
		mRcId = rc_id;
	}

	std::string movementMode() const
	{
		return mMovementMode;
	}

	void movementMode(const std::string& m)
	{
		mMovementMode = m;
	}


private:

	std::string mMovementMode;
	std::string mTeamname;
	std::string mNickname;
	int         mVtId;
	int         mRcId;

};

class ServerProperties {

public:

	ServerProperties()
	{

	}

	ServerProperties(XmlProperties& props)
		: mXmlProperties(props)
	{

	}

	std::string Get(const std::string& k) const {
		return BasicGet(k);
	}

	int GetInt(const std::string& k) const {
		return convert_to<int>(BasicGet(k));
	}

	float GetFloat(const std::string& k) const {
		return convert_to<float>(BasicGet(k));
	}

	double GetDouble(const std::string& k) const {
		return convert_to<double>(BasicGet(k));
	}

private:

	std::string BasicGet(const std::string& k) const
	{
		if (mXmlProperties.Exists(k))
		{
			return mXmlProperties.Value(k);
		}
		else
		{
			throw std::logic_error("Request for non-existent flag " + k);
		}
	}

private:

	XmlProperties mXmlProperties;

};

class Client
{
public:
    /*
     * Constructor reads configFile and sets client configurable attributes.
     */
    Client();
    Client(int uid, const std::string& nickname);
	~Client();

	mrTeam getTeam();
	int vtID() const;
	int rcID() const;
	std::string nickname() const;
	mrTeam Team() const;
	std::string version() const;
	std::string teamname() const;

private:

	int mUid;
    mrTeam mTeam;
    int mVtID;
    int mRcID;

    std::string mTeamName;
    std::string mNickname;

};

#endif /*CLIENT_H_*/
