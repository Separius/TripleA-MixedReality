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

#include <Xml_Connection.h>

bool const
ServerToClientConnect :: write(std::ostream* doc, unsigned int indent)
{
    XMLSerializer::writeLeafNode(doc, "type", mType, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "protocol_version", mProtocolVersion, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "movement_mode", mMovementMode, NULL, indent);
    return true;
}

bool const
ServerToClientConnect :: read(const TLDocumentSlice& slice)
{
    vector<XMLItem> items;
    XMLSerializer::itemize(slice, items);

    std::vector<XMLItem>::iterator it = items.begin();
    for (; it != items.end(); ++it)
    {
    	if (it->getName() == "type")
    	{
    		mType = it->getContent();
    	}
    	else if (it->getName() == "protocol_version")
    	{
    		mProtocolVersion = it->getContent();
    	}
    	else if (it->getName() == "movement_mode")
    	{
    		mMovementMode = it->getContent();
    	}
    }

    return true;
}

bool const
ClientToServerConnect :: write(std::ostream* doc, unsigned int indent)
{
    XMLSerializer::writeLeafNode(doc, "type", mType, NULL, indent);
    XMLSerializer::writeLeafNode(doc, "protocol_version", mProtocolVersion, NULL, indent);
    return true;
}

bool const
ClientToServerConnect :: read(const TLDocumentSlice& slice)
{
    vector<XMLItem> items;
    XMLSerializer::itemize(slice, items);

	std::vector<XMLItem>::iterator it;
	for (it = items.begin(); it != items.end(); ++it)
	{
		if (it->getName() == "type")
		{
			mType = it->getContent();
		}
		else if (it->getName() == "protocol_version")
		{
			mProtocolVersion = it->getContent();
		}
	}

    return false;
}

/*virtual*/
bool const
ServerToClientParameters::write(std::ostream* doc, unsigned int indent)
{
	XMLSerializer::writeNestedNode(doc, mProps, indent);
	return true;
}

/*virtual*/
bool const
ServerToClientParameters::read(const TLDocumentSlice& slice)
{
	std::vector<XMLItem> items;
	XMLSerializer::itemize(slice, items);

	std::vector<XMLItem>::iterator it;
	for (it = items.begin(); it != items.end(); ++it)
	{
		if (it->getName() == mProps.getName())
		{
			if (!mProps.read(*it))
			{
				return false;
			}
		}
	}

	return true;
}

/*virtual*/
bool const ClientToServerParameters::write(std::ostream* doc, unsigned int indent)
{
	XMLSerializer::writeLeafNode(doc, "rc_id", mRcId, NULL, indent);
	XMLSerializer::writeLeafNode(doc, "vt_id", mVtId, NULL, indent);
	XMLSerializer::writeLeafNode(doc, "nickname", mNickName, NULL, indent);
	XMLSerializer::writeLeafNode(doc, "teamname", mTeamName, NULL, indent);
	XMLSerializer::writeLeafNode(doc, "movement_mode", mMovementMode, NULL, indent);

	return true;
}

/*virtual*/
bool const ClientToServerParameters::read(const TLDocumentSlice& slice)
{
    std::vector<XMLItem> items;
    XMLSerializer::itemize(slice, items);

    for (unsigned int i = 0 ; i < items.size() ; ++i)
    {
    	if (items[i].getName() == "rc_id")
	    {
	    	mRcId = items[i].getContent_int();
	    }
	    else if (items[i].getName() == "vt_id")
	    {
	    	mVtId = items[i].getContent_int();
	    }
	    else if (items[i].getName() == "nickname")
	    {
	    	mNickName = items[i].getContent();
	    }
	    else if (items[i].getName() == "teamname")
	    {
	    	mTeamName = items[i].getContent();
	    }
	    else if (items[i].getName() == "movement_mode")
	    {
	    	mMovementMode = items[i].getContent();
	    }
    }

    return true;
}
