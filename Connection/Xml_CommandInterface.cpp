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

#include "Xml_CommandInterface.h"

#include <cmath>
#include <fstream>

CommandInterface::CommandInterface()
{
}

CommandInterface::~CommandInterface()
{
}

CommandType :: CommandType()
{
    mCommandName = "";
    miAngle = 0;
    mDist = 0;
    mdAngle = mStrength = mRightVelocity = mLeftVelocity = 0.0;
}

/* Setup the wheel velocities.
 */
void
Command :: setVelocity(float dLeftWheel, float dRightWheel)
{
    mCommandType.mCommandName = "wheel_velocities";
    mCommandType.mCommandEnum = WHEEL_VELOCITY;
    mCommandType.mRightVelocity = dRightWheel;
    mCommandType.mLeftVelocity = dLeftWheel;
}

/* Setup a kick command
 */
void
Command :: setKick(double angle, double strength)
{
    mCommandType.mCommandName = "kick";
    mCommandType.mCommandEnum = KICK;
    mCommandType.mdAngle = 180 * angle / M_PI;
    mCommandType.mStrength = strength;
}

void
Command :: setSteeringCommand(unsigned int dist, int angle)
{
    mCommandType.mCommandName = "goto";
    mCommandType.mCommandEnum = STEERING;
    mCommandType.miAngle = angle;
    mCommandType.mDist = dist;
}

bool const
CommandType::write(std::ostream* doc, unsigned int indent)
{
    switch(mCommandEnum){
    case WHEEL_VELOCITY:
        XMLSerializer::writeLeafNode(doc, "right", mRightVelocity, NULL, indent);
        XMLSerializer::writeLeafNode(doc, "left", mLeftVelocity, NULL, indent);
        break;
    case KICK:
        XMLSerializer::writeLeafNode(doc, "angle",  mdAngle , NULL, indent);
        XMLSerializer::writeLeafNode(doc, "force", mStrength, NULL, indent);
        std::cout << mdAngle << std::endl << std::endl;
        break;
    case STEERING:
        XMLSerializer::writeLeafNode(doc, "dist", mDist, NULL, indent);
        XMLSerializer::writeLeafNode(doc, "angle", miAngle, NULL, indent);
        break;
    default:
        return false;
    }
    return true;
}

bool const
CommandType::read(const TLDocumentSlice& slice)
{
    vector<XMLItem> items;
    XMLSerializer::itemize(slice, items);

    //kick
    if ( items[0].getName() == "angle"
      && items[1].getName() == "force")
    {
        mdAngle = items[0].getContent_float();
        mStrength = items[1].getContent_float();
        return true;
    }

    //goto
    if ( items[0].getName() == "dist"
      && items[1].getName() == "angle")
    {
        mDist = items[0].getContent_int();
        miAngle = items[1].getContent_int();
        return true;
    }

    //wheel_velocities
    if ( items[0].getName() == "right"
      && items[1].getName() == "left")
    {
        mRightVelocity = items[0].getContent_float();
        mLeftVelocity = items[1].getContent_float();
        return true;
    }
    return false;
}

bool const
Command::write(std::ostream* doc, unsigned int indent)
{
    XMLSerializer::writeNestedNode(doc, mCommandType, indent);
    return true;
}

bool const
Command::read(const TLDocumentSlice& slice)
{
    vector<XMLItem> items;
    XMLSerializer::itemize(slice, items);

    if ( items[0].getItemType() == XML_NESTED){
        mCommandType.read(items[0]);
        return true;
    }
    return false;
}
