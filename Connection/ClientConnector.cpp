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

#include "ClientConnector.h"

#include "Connection.h"
#include "Client.h"

ClientConnector::ClientConnector(const std::string& host, int port)
	: mHost(host)
	, mPort(port)
	, mTries(20)
	, mRetryTime(5000)
{
	mUdpConnection = new Connection();
	mUdpConnection->Init(mHost, mPort);
}

int ClientConnector::tries() const
{
	return mTries;
}

void ClientConnector::tries(int tries)
{
	mTries = tries;
}

int ClientConnector::retryTime() const
{
	return mRetryTime;
}

void ClientConnector::retryTime(int a_time)
{
	mRetryTime = a_time;
}

ClientConnector::~ClientConnector() {

}

Connection*
ClientConnector::getConnection()
{
    return mUdpConnection;
}

std::string
ClientConnector::generateMsgConnectServer(const ConnectionParameters& params)
{

    ClientToServerConnect connectXml;
    connectXml.mProtocolVersion = "1.1";
    connectXml.mType = "Client";

    stringstream connectMsg;
    XMLSerializer::writeNestedNode(&connectMsg, &connectXml);
    return connectMsg.str();
}

ServerProperties
ClientConnector::ConnectToServer(const ConnectionParameters& params)
{
	std::string msg = generateMsgConnectServer(params);
    mUdpConnection->sendConnectionRequest(msg);

    int tries = mTries;
    int sleep = mRetryTime;

    bool received_response = false;
    bool waiting_parameters = false;

    /* Here we wait for an answer from the server and, in case it is "true",
     * we wait for a connection request from it and send an answer.
     */
    std::cout << "Trying " << tries << " times " << std::endl;
    std::cout << "Sleeping " << sleep << " usecs" << std::endl;

    while (tries)
    {
        if( mUdpConnection->ReceiveOne() )
        {
            /*
             * trying to receive an answer to our request.
             */
            std::string aux = mUdpConnection->getData();

            if (!received_response)
	    	{
	            if(mUdpConnection->receiveConnectionResponse(aux))
	            {
	            	std::cout << "Received response for connection request" << std::endl;
	                received_response = true;
			    	continue;
	            }

            }
            else if (!waiting_parameters)
            {
                /**
                 *  If our request was accepted, we wait a
                 * connection request from the server.
                 */
                ServerToClientConnect serverRequest;
                std::string aux = mUdpConnection->getData();
                std::stringstream answer(aux);

                XMLItem root = XMLSerializer::documentToRoot(&answer);

                if (serverRequest.read(root))
                {
                	mUdpConnection->sendConnectionResponse(true);
                }
                else
                {
					/**
					 * Something is wrong... refuse connection.
					 */
                	mUdpConnection->sendConnectionResponse(false);
                	throw ClientException("Server sent invalid message: \"" + aux + "\"");
                }

                std::cout << "Sent connection responde to server, waiting for parameters" << std::endl;
                waiting_parameters = true;
	    	}
            else
            {
            	// The server will send the properties
                ServerToClientParameters server_params;
                std::string data = mUdpConnection->getData();
                std::stringstream answer(data);
                XMLItem root = XMLSerializer::documentToRoot(&answer);

                if (server_params.read(root))
                {

                	std::cout << "Received parameters from server" << std::endl;
                }
                else
                {
                	throw ClientException("Invalid message received from server \""
								+ data + "\"");
                }

                /**
			    * Sending client parameters
			    */
                ClientToServerParameters expected_params;
                expected_params.mRcId = params.rcId();
                expected_params.mVtId = params.vtId();
                expected_params.mTeamName = params.teamname();
                expected_params.mNickName = params.nickname();
                expected_params.mMovementMode = params.movementMode();

                stringstream client_params_stream;
                XMLSerializer::writeNestedNode(&client_params_stream, &expected_params);

                mUdpConnection->sendConnectionRequest(
                		client_params_stream.str());


                /**
                 * Returning properties
                 */
                ServerProperties serverProps(server_params.mProps);
            	return serverProps;
            }
        }

        tries--;
        usleep(sleep);
    }

    throw ClientException("Could not connect to "
				+ mHost + ":"
				+ convert_to<std::string>(mPort)
				+ " after "
				+ convert_to<std::string>(mTries)
				+ " tries");
}

