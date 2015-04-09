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

#ifndef XML_COMMANDINTERFACE_H_
#define XML_COMMANDINTERFACE_H_

#include <sstream>

#include <DeluxeML/XMLSerializable.h>
#include <DeluxeML/XMLSerializer.h>
#include <DeluxeML/XMLItem.h>

typedef enum __commandType{ WHEEL_VELOCITY = 1,
                            KICK,
                            STEERING}_commandType;

class CommandInterface
{
public:
	CommandInterface();
	virtual ~CommandInterface();	
};

class CommandType : public XMLSerializable
{
public:
    
    CommandType();
    virtual bool const write(std::ostream* doc, unsigned int indent=0);
    
    virtual bool const read(const TLDocumentSlice& slice);
        
    virtual std::string const getName() const { return mCommandName; };
    
    std::string mCommandName;
    _commandType mCommandEnum;
    float mStrength;
    float mRightVelocity,mLeftVelocity;
    unsigned int mDist;
    /* For steering commands */
    int miAngle;
    /* For kick commands */
    float mdAngle;
};

class Command : public XMLSerializable
{
public:
    virtual bool const write(std::ostream* doc, unsigned int indent=0);

    virtual bool const read(const TLDocumentSlice& slice);  
        
    virtual std::string const getName() const { return "command"; }
    
    void setVelocity(float dLeftWheel, float dRightWheel);
    void setKick(double angle, double strength);
    void setSteeringCommand(unsigned int dist, int angle);

    CommandType mCommandType;
};

#endif /*XML_COMMANDINTERFACE_H_*/
