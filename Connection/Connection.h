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

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <iostream>
#include <string>
#include <SDL/SDL_net.h>

#define MAX_MESSAGE_LEN 4*1024

class Connection
{
public:
	Connection();
	virtual ~Connection();

	/* Opens up a connection to the given
     * host and port
     */
    bool Init(std::string pServer, int pPort);

    /* Sends the given string through the
     * connection
     */
    bool Send(std::string pMsg);

    /* Sends message to server requesting connection.
     */
    bool sendConnectionRequest(std::string request);
    /* Answer a connection request.
     */
    bool sendConnectionResponse(bool value);

    /**
     * Receive data from the connection and stores internally
     */
    bool Receive();
    /**
     * Reads only one data from the buffer
     */
    bool ReceiveOne();

    /* Reads a request answer from the server.
     */
    bool receiveConnectionResponse(std::string srvMsg);

    /* Returns the stored data last received
     */
    std::string getData();

    /* Close the UDP socket
     */
    void Close();

private:

    /* Server IP
     */
    IPaddress mServerAddress;

    /* Socket
     */
    UDPsocket mSocket;

    /* Pointers to send and receive buffers
     */
    UDPpacket *mSendBuffer;
    UDPpacket *mReceiveBuffer;

    /* Tells wheter there is already a connection or not
     */
    bool mWasInit;
};

#endif /*CONNECTION_H_*/
