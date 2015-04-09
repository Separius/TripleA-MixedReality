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

/*
 * This class is mainly based on UDPConnection class from older Rodrigo Guerra's
 * pv-soccer-client.
 * Provides methods for connecting the client to the soccer server.
 *
 */

#include "Connection.h"
#include <DeluxeML/XMLItem.h>
#include <DeluxeML/XMLSerializer.h>

/* Constructor
 */
Connection :: Connection()
{
    mWasInit = false;
}

/* Destructor
 */
Connection :: ~Connection()
{
    /* Check if we have something to be freed
     */
    if (mWasInit) Close();
    SDLNet_Quit();
}

/* Close the UDP socket
 */
void
Connection :: Close()
{
    mWasInit = false;
    SDLNet_UDP_Close(mSocket);
    SDLNet_FreePacket(mSendBuffer);
    SDLNet_FreePacket(mReceiveBuffer);
}

/* Opens up a connection to the given
 * host and port
 */
bool
Connection :: Init(std::string pServer, int pPort)
{
    /* Initialize SDL_net
     */
    if (SDLNet_Init() < 0)
    {
        std :: cerr << "SDLNet_Init: "
                    << SDLNet_GetError()
                    << std :: endl;
        return false;
    }

    /* Allocate memory for the packets
     */
    if (!(mSendBuffer = SDLNet_AllocPacket(MAX_MESSAGE_LEN)))
    {
        std :: cerr << "SDLNet_AllocPacket: "
                    << SDLNet_GetError()
                    << std :: endl;
        return false;
    }
    if (!(mReceiveBuffer = SDLNet_AllocPacket(MAX_MESSAGE_LEN)))
        {
            std :: cerr << "SDLNet_AllocPacket: "
                        << SDLNet_GetError()
                        << std :: endl;
            return false;
    }

    /* Open the socket at a random port
     */
    if (!(mSocket = SDLNet_UDP_Open(0)))
    {
        std :: cerr << "SDLNet_UDP_Open: "
                    << SDLNet_GetError()
                    << std :: endl;
        return false;
    }

    /* Resolve the server name
     */
    if (SDLNet_ResolveHost(& mServerAddress, pServer.c_str(), pPort))
    {
        std :: cerr << "SDLNet_ResolveHost("
                    << pServer
                    << ","
                    << pPort
                    << ") "
                    << SDLNet_GetError();
        return false;
    }

    mWasInit = true;

    return true;
}

/* Sends the given string through the
 * connection
 */
bool
Connection :: Send(std::string pMsg)
{
    //cout << "pMsg:  " << pMsg << endl;
    /* Put the message into the packet data buffer
     */
    strcpy((char *)mSendBuffer->data, pMsg.c_str());
    /* Setup destination
     */
    mSendBuffer->address.host =  mServerAddress.host;
    mSendBuffer->address.port =  mServerAddress.port;

    /* Set length field
     */
    mSendBuffer->len = pMsg.size() + 1;

    /* Send the data
     */
    return SDLNet_UDP_Send(mSocket, -1, mSendBuffer);
}

bool
Connection :: sendConnectionRequest(std::string request)
{
    return Send(request);
}

bool
Connection::sendConnectionResponse(bool value)
{
    std::string response = std::string("<connect>")
                         + (value ? "true" : "false")
                         + std::string("</connect>");

   return Send(response);
}

/* Receive data from the connection and stores internally
 */
bool
Connection :: Receive()
{
    bool ret = false;
    while (ReceiveOne()){
        ret = true;
    }
    return ret;
}

bool
Connection :: ReceiveOne()
{
	if (SDLNet_UDP_Recv(mSocket, mReceiveBuffer))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool
Connection :: receiveConnectionResponse(std::string srvMsg)
{
    if(srvMsg == "")
    {
        std::cout<<"Connection answer is empty!! Something is wrong with the client."
                 << std::endl;
        return false;
    }

    std::stringstream inStream(srvMsg);

    XMLItem item = XMLSerializer::documentToRoot(&inStream);

    if (item.getItemType() == XML_NESTED
     || item.getItemType() == XML_ERROREOUS)
    {
        std::cout << "Connection error: answer message corrupted."
                  << std::endl;
    }

    if (item.getName() == "connect"
     && item.getContent() == "true")
    {
        return true;
    }
    else if (item.getName() == "connect"
          && item.getContent() == "false")
    {
        std::cout << "Connection refused by the Soccer Server"
                  << std::endl;
    }

    std::cout << "Invalid message received" << std::endl;
    return false;
}

/* Returns the stored data last received
 */
std::string
Connection :: getData()
{
	std::string s;

	for (int i = 0; i < mReceiveBuffer->len; i++)
	{
		s += static_cast<char>(mReceiveBuffer->data[i]);
	}

    return s;
}
