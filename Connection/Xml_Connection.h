/*
 * Written by Juliana Fajardini <jufajardini@gmail.com>,
 * basead on soccer server xml classes, made by
 * Adailton Cerqueira <adailton.junior@gmail.com>,
 * Elen Casaes <elenmargareth@gmail.com>
 * and José Grimaldo da Silva <jose.jgrimaldo@gmail.com>
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

#ifndef XML_CONNECTION_H_
#define XML_CONNECTION_H_

#include <sstream>
#include <fstream>
#include <vector>

#include <DeluxeML/XMLSerializable.h>
#include <DeluxeML/XMLSerializer.h>
#include <DeluxeML/XMLItem.h>

#include "XmlProperties.h"

class ClientToServerConnect : public XMLSerializable
{
public:
    virtual bool const write(std::ostream* doc, unsigned int indent=0);
    virtual bool const read(const TLDocumentSlice& slice);
    virtual std::string const getName() const { return "connect"; }

    std::string mType;
    std::string mProtocolVersion;
};

class ServerToClientConnect : public XMLSerializable
{
public:
    virtual bool const write(std::ostream* doc, unsigned int indent=0);

    virtual bool const read(const TLDocumentSlice& slice);
    virtual std::string const getName() const { return "connect"; }

    std::string mType;
    std::string mProtocolVersion;
    std::string mMovementMode;
};


class ServerToClientParameters  : public XMLSerializable {

public:
	virtual std::string const getName() const { return "parameters"; }
    virtual bool const write(std::ostream* doc, unsigned int indent=0);
    virtual bool const read(const TLDocumentSlice& slice);

public:

	XmlProperties mProps;
};

class ClientToServerParameters : public XMLSerializable {
public:

	virtual std::string const getName() const { return "parameters"; }
    virtual bool const write(std::ostream* doc, unsigned int indent=0);
    virtual bool const read(const TLDocumentSlice& slice);

public:
    std::string mNickName;
	std::string mTeamName;
	std::string mMovementMode;
	int mRcId;
	int mVtId;
};

#endif /*XML_CONNECTION_H_*/
